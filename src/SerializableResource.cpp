#include "SerializableResource.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SerializableResource::_bind_methods()
{
    ClassDB::add_virtual_method(
        "SerializableResource",
        MethodInfo(Variant::DICTIONARY, "serialize"),
        {"self"});
    ClassDB::add_virtual_method(
        "SerializableResource",
        MethodInfo("deserialize", PropertyInfo(Variant::DICTIONARY, "data")),
        {"self", "data"});
}