#pragma once

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "SerializableResource.h"

using namespace godot;

Dictionary SerializeSerializableResource(Object &obj);
Ref<SerializableResource> DeserializeSerializableResource(const Dictionary &data);

Variant SerializeRecursive(const Variant &var);
Variant DeserializeRecursive(const Variant &var);