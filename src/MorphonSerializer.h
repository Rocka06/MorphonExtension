#pragma once

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include "SerializableResource.h"

using namespace godot;

class MorphonSerializer : public Object
{
    GDCLASS(MorphonSerializer, Object)

protected:
    static void _bind_methods();

public:
    static HashMap<String, String> RegisteredScripts;

    static void RegisterScript(const String &name, const Ref<Script> &script);

    static Dictionary SerializeSerializableResource(Object &obj);
    static Ref<SerializableResource> DeserializeSerializableResource(const Dictionary &data);

    static Variant SerializeRecursive(const Variant &var);
    static Variant DeserializeRecursive(const Variant &var);

    static bool IsValidPath(const String &path);
};
