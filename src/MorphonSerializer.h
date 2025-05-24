#include "SerializableResource.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

void save_serializables(const Array &objects, const String &path)
{
    Array serialized_array;

    // Serialize each object via the virtual method
    for (int i = 0; i < objects.size(); i++)
    {
        Object *obj = Object::cast_to<Object>(objects[i]);
        if (!obj)
        {
            continue;
        }

        // Cast to ISerializable interface
        SerializableResource *serializable = Object::cast_to<SerializableResource>(obj);
        if (!serializable)
        {
            continue;
        }

        // Call serialize (virtual method exposed via add_virtual_method)
        Dictionary dict = serializable->serialize();

        serialized_array.append(dict);
    }

    // Convert to JSON string
    String json_string = JSON::print(serialized_array);

    // Save JSON string to file
    Ref<FileAccess> file = FileAccess::open(path, FileAccess::WRITE);
    if (!file)
    {
        Godot::print_error("Failed to open file for writing: " + path, __FUNCTION__, __FILE__, __LINE__);
        return;
    }

    file->store_string(json_string);
    file->close();
}
