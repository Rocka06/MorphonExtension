#pragma once

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class SerializableResource : public Resource
{
    GDCLASS(SerializableResource, Resource)

protected:
    static void _bind_methods();

public:
    virtual Dictionary _serialize();
    virtual void _deserialize(const Dictionary &data) {}
};