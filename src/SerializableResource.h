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
    virtual Dictionary serialize()
    {
        Dictionary dict;
        return dict;
    }
    virtual void deserialize(const Dictionary &data)
    {
    }
};