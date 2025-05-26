#pragma once
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include "SerializableResource.h"
#include "MorphonSerializer.h"

using namespace godot;

class MorphonConfigFile : public RefCounted
{
    GDCLASS(MorphonConfigFile, RefCounted)

private:
    HashMap<String, HashMap<String, Variant>> m_Values;

protected:
    static void _bind_methods();

public:
    void set_value(const String &p_section, const String &p_key, const Variant &p_value);
    Variant get_value(const String &p_section, const String &p_key, const Variant &p_default = nullptr) const;
    bool has_section(const String &p_section) const;
    bool has_section_key(const String &p_section, const String &p_key) const;
    PackedStringArray get_sections() const;
    PackedStringArray get_section_keys(const String &p_section) const;
    void erase_section(const String &p_section);
    void erase_section_key(const String &p_section, const String &p_key);
    Error parse(const String &data);
    Error load(const String &p_path);
    Error load_encrypted(const String &p_path, const PackedByteArray &p_key);
    Error load_encrypted_pass(const String &p_path, const String &p_password);
    String encode_to_text() const;
    Error save(const String &p_path) const;
    Error save_encrypted(const String &p_path, const PackedByteArray &p_key) const;
    Error save_encrypted_pass(const String &p_path, const String &p_password) const;
    void clear();
};