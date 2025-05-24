#pragma once

#include <godot_cpp/classes/sprite2d.hpp>

using namespace godot;

class SerializableResource : public Resource
{
    GDCLASS(SerializableResource, Resource)

protected:
    static void _bind_methods();

public:
    virtual Dictionary serialize() const = 0;
    virtual void deserialize(const Dictionary &data) = 0;
};