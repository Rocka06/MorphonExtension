#include "SerializableResource.h"
#include <godot_cpp/core/class_db.hpp>

Dictionary SerializableResource::_serialize()
{
    Dictionary dict;
    return dict;
}

void SerializableResource::_bind_methods()
{
    ClassDB::add_virtual_method("SerializableResource", MethodInfo(Variant::DICTIONARY, "_serialize"));
    ClassDB::add_virtual_method("SerializableResource", MethodInfo("_deserialize", PropertyInfo(Variant::DICTIONARY, "data")));
}