#include "MorphonSerializer.h"

Dictionary SerializeSerializableResource(SerializableResource &res)
{
    Dictionary data = res.call("serialize");
    Ref<Script> s = res.get_script();
    data["ScriptPath"] = s->get_path();
    return data;
}

Ref<SerializableResource> DeserializeSerializableResource(const Dictionary &data)
{
    if (!data.has("ScriptPath"))
        return nullptr;

    String path = data["ScriptPath"];

    if (!path.begins_with("res://"))
        return nullptr;

    Ref<Script> script = ResourceLoader::get_singleton()->load(path);

    if (!script.is_valid())
        return nullptr;

    Ref<SerializableResource> res = memnew(SerializableResource);
    res->set_script(script);

    res->call("deserialize", data);
    return res;
}

Variant SerializeRecursive(const Variant &var)
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

        if (Object::cast_to<Resource>(obj))
        {
            Resource *res = Object::cast_to<Resource>(obj);
            if (res->is_local_to_scene())
                return nullptr;

            return res->get_path();
        }

        return nullptr;
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
    default:
        return var;
        break;
    }
}

Variant DeserializeRecursive(const Variant &var)
{
    switch (var.get_type())
    {
    case Variant::STRING:
    {
        String str = var;
        if (str.begins_with("res://"))
        {
            return ResourceLoader::get_singleton()->load(str);
        }
    }
    case Variant::DICTIONARY:
    {
        Dictionary dict = var;

        if (dict.has("ScriptPath"))
        {
            return DeserializeSerializableResource(dict);
        }

        Dictionary result;
        Array keys = dict.keys();
        for (int i = 0; i < keys.size(); ++i)
        {
            Variant key = DeserializeRecursive(keys[i]);
            Variant val = DeserializeRecursive(dict[keys[i]]);
            result[key] = val;
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