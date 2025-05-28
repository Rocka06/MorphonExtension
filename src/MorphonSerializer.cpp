#include "MorphonSerializer.h"

HashMap<String, Ref<Script>> MorphonSerializer::RegisteredScripts;

void MorphonSerializer::RegisterScript(const String &name, const Ref<Script> &script)
{
    if (!RegisteredScripts.has(name))
    {
        RegisteredScripts.insert(name, script, RegisteredScripts.is_empty());
    }
}

Dictionary MorphonSerializer::SerializeSerializableResource(Object &obj)
{
    Dictionary data = obj.call("_serialize");
    data = SerializeRecursive(data);
    Ref<Script> s = obj.get_script();

    for (const KeyValue<String, Ref<Script>> &kv : RegisteredScripts)
    {
        if (kv.value->get_path() == s->get_path())
        {
            data["._typeName"] = kv.key;
            return data;
        }
    }

    ERR_FAIL_V_MSG(Dictionary(), "Script \"" + s->get_path() + "\" has not been registered! Register it with MorphonSerializer.RegisterScript(name, script)");
}

Ref<SerializableResource> MorphonSerializer::DeserializeSerializableResource(const Dictionary &data)
{
    if (!data.has("._typeName"))
        return nullptr;

    String type = data["._typeName"];
    for (auto &i : RegisteredScripts)
    {
        if (i.key == type)
        {
            Ref<Script> script = i.value;
            Ref<SerializableResource> res;
            res.instantiate();

            res->set_script(script);
            res->call("_deserialize", data);
            return res;
        }
    }

    ERR_FAIL_V_MSG(nullptr, "Type \"" + type + "\" has not been registered! Register it with MorphonSerializer.RegisterScript(name, script)");
    return nullptr;
}

Variant MorphonSerializer::SerializeRecursive(const Variant &var)
{
    switch (var.get_type())
    {
    case Variant::OBJECT:
    {
        Object *obj = Object::cast_to<Object>(var);

        if (!obj)
            return nullptr;

        if (Object::cast_to<SerializableResource>(obj))
        {
            SerializableResource *res = Object::cast_to<SerializableResource>(obj);
            return SerializeSerializableResource(*res);
        }
        else if (Object::cast_to<Resource>(obj))
        {
            Resource *res = Object::cast_to<Resource>(obj);

            // CSharp binding
            if (res->has_method("_serialize") && res->has_method("_deserialize"))
                return SerializeSerializableResource(*res);

            if (res->is_local_to_scene())
                return nullptr;

            return res->get_path();
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

    return var;
}

Variant MorphonSerializer::DeserializeRecursive(const Variant &var)
{
    switch (var.get_type())
    {
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
            return DeserializeSerializableResource(result);
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
    default:
        return var;
    }
}

void MorphonSerializer::_bind_methods()
{
    ClassDB::bind_static_method("MorphonSerializer", D_METHOD("RegisterScript", "name", "script"), &MorphonSerializer::RegisterScript);
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