#pragma once

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

class MorphonSerializer : public Object
{
    GDCLASS(MorphonSerializer, Object)

protected:
    static void _bind_methods();

public:
    static HashMap<String, String> RegisteredScripts;

    static void RegisterScript(const String &name, const Ref<Script> &script);
    static void MorphonSerializer::RegisterScriptByPath(const String &name, const String &scriptPath);

    static Dictionary SerializeResource(const Resource &res);
    static Ref<Resource> DeserializeResource(const Dictionary &data);

    static Variant SerializeRecursive(const Variant &var);
    static Variant DeserializeRecursive(const Variant &var);

    static Dictionary GetResourceProperties(const Resource &res);

    static Ref<Script> GetRegisteredScript(const String &name);

    static bool IsValidPath(const String &path);
};
