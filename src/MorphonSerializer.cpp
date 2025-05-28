#include "MorphonSerializer.h"

HashMap<String, String> MorphonSerializer::RegisteredScripts;

void MorphonSerializer::RegisterScript(const String &name, const Ref<Script> &script)
{
    if (!RegisteredScripts.has(name))
    {
        RegisteredScripts.insert(name, script->get_path(), RegisteredScripts.is_empty());
        return;
    }

    ERR_FAIL_MSG("You have already registered a script named \"" + name + "\"");
}
void MorphonSerializer::RegisterScriptByPath(const String &name, const String &scriptPath)
{
    ERR_FAIL_COND_MSG(RegisteredScripts.has(name), "You have already registered a script named \"" + name + "\"");

    RegisteredScripts.insert(name, scriptPath, RegisteredScripts.is_empty());
}

Dictionary MorphonSerializer::SerializeResource(const Resource &res)
{
    Dictionary data = GetResourceProperties(res);
    data = SerializeRecursive(data);

    Ref<Script> s = res.get_script();
    for (const KeyValue<String, String> &kv : RegisteredScripts)
    {
        if (kv.value == s->get_path())
        {
            data["._typeName"] = kv.key;
            return data;
        }
    }

    ERR_FAIL_V_MSG(Dictionary(), "Script \"" + s->get_path() + "\" has not been registered! Register it with MorphonSerializer.RegisterScript(name, script)");
}
Ref<Resource> MorphonSerializer::DeserializeResource(const Dictionary &data)
{
    if (data.is_empty())
        return nullptr;

    if (!data.has("._typeName"))
        return nullptr;

    String type = data["._typeName"];
    Ref<Script> script = GetRegisteredScript(type);

    if (script == nullptr)
        ERR_FAIL_V_MSG(nullptr, "Type \"" + type + "\" has not been registered! Register it with MorphonSerializer.RegisterScript(name, script)");

    Ref<Resource> res;
    res.instantiate();
    res->set_script(script);

    // We build the resource based on the property list not on the save data
    Dictionary properties = GetResourceProperties(*res.ptr());
    TypedArray<String> keys = properties.keys();

    for (int i = 0; i < keys.size(); i++)
    {
        String key = keys[i];
        res->set(key, data[key]);
    }

    return res;
}

Variant MorphonSerializer::SerializeRecursive(const Variant &var)
{
    switch (var.get_type())
    {
    case Variant::NIL:
    case Variant::BOOL:
    case Variant::INT:
    case Variant::FLOAT:
    case Variant::STRING:
    case Variant::STRING_NAME:
        return var;
    case Variant::OBJECT:
    {
        Object *obj = Object::cast_to<Object>(var);

        if (!obj)
            return nullptr;

        if (Object::cast_to<Resource>(obj))
        {
            Resource *res = Object::cast_to<Resource>(obj);

            // Check if it is a custom resource or a built in one
            if (res->get_class() == "Resource")
                return SerializeResource(*res);

            if (!res->is_local_to_scene())
                return res->get_path();

            return nullptr;
        }
        break;
    }
    case Variant::DICTIONARY:
    {
        Dictionary dict = var;
        Dictionary result;
        Array keys = dict.keys();
        for (int i = 0; i < keys.size(); ++i)
        {
            Variant key = keys[i];
            // Variant serialized_key = SerializeRecursive(key);
            Variant serialized_value = SerializeRecursive(dict[key]);
            // result[serialized_key] = serialized_value;
            result[key] = serialized_value;
        }
        return result;
    }
    case Variant::ARRAY:
    {
        Array arr = var;
        Array result;
        for (int i = 0; i < arr.size(); ++i)
        {
            result.append(SerializeRecursive(arr[i]));
        }
        return result;
    }
    }

    return JSON::from_native(var);
}
Variant MorphonSerializer::DeserializeRecursive(const Variant &var)
{
    switch (var.get_type())
    {
    case Variant::NIL:
    case Variant::BOOL:
    case Variant::INT:
    case Variant::FLOAT:
    case Variant::STRING_NAME:
        return var;
    case Variant::STRING:
    {
        String str = var;
        if (str.begins_with("res://"))
        {
            if (str.to_lower().ends_with(".gd") || str.to_lower().ends_with(".cs"))
                return nullptr;

            if (IsValidPath(str))
                return ResourceLoader::get_singleton()->load(str);

            return nullptr;
        }

        return var;
    }
    case Variant::DICTIONARY:
    {
        Dictionary dict = var;

        Dictionary result;
        Array keys = dict.keys();
        for (int i = 0; i < keys.size(); ++i)
        {
            if (keys[i] == "._typeName")
            {
                // If we are deserializing a SerializableResources properties, we want to keep this one
                result[keys[i]] = dict[keys[i]];
                continue;
            }

            Variant key = DeserializeRecursive(keys[i]);
            Variant val = DeserializeRecursive(dict[keys[i]]);
            result[key] = val;
        }

        if (dict.has("._typeName"))
        {
            return DeserializeResource(result);
        }
        return result;
    }
    case Variant::ARRAY:
    {
        Array arr = var;
        Array result;
        for (int i = 0; i < arr.size(); ++i)
        {
            result.append(DeserializeRecursive(arr[i]));
        }
        return result;
    }
    }

    return JSON::from_native(var);
}

Dictionary MorphonSerializer::GetResourceProperties(const Resource &res)
{
    Dictionary result;

    TypedArray<Dictionary> properties = res.get_property_list();
    for (int i = 0; i < properties.size(); i++)
    {
        Dictionary property = properties[i];
        if ((int)property["usage"] & PROPERTY_USAGE_SCRIPT_VARIABLE)
        {
            result[property["name"]] = res.get(property["name"]);
        }
    }

    return result;
}
Ref<Script> MorphonSerializer::GetRegisteredScript(const String &name)
{
    String *path = RegisteredScripts.getptr(name);

    if (!path)
        return nullptr;

    if (!IsValidPath(*path))
        return nullptr;

    return ResourceLoader::get_singleton()->load(*path);
}

bool MorphonSerializer::IsValidPath(const String &path)
{
    if (!path.begins_with("res://"))
        return false;

    if (path.find("..") != -1)
        return false;

    String abs_path = ProjectSettings::get_singleton()->globalize_path(path);
    String abs_root = ProjectSettings::get_singleton()->globalize_path("res://");

    if (!abs_path.begins_with(abs_root))
        return false;

    return true;
}

void MorphonSerializer::_bind_methods()
{
    ClassDB::bind_static_method("MorphonSerializer", D_METHOD("register_script", "name", "script"), &MorphonSerializer::RegisterScript);
    ClassDB::bind_static_method("MorphonSerializer", D_METHOD("register_script_by_path", "name", "script_path"), &MorphonSerializer::RegisterScriptByPath);
}