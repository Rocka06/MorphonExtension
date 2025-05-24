#include "MorphonConfigFile.h"

void MorphonConfigFile::set_value(const String &p_section, const String &p_key, const Variant &p_value)
{
    if (p_value.get_type() == Variant::NIL)
    { // Erase key.
        if (!values.has(p_section))
        {
            return;
        }

        values[p_section].erase(p_key);
        if (values[p_section].is_empty())
        {
            values.erase(p_section);
        }

        return;
    }

    if (!values.has(p_section))
    {
        // Insert section-less keys at the beginning.
        values.insert(p_section, HashMap<String, Variant>(), p_section.is_empty());
    }

    values[p_section][p_key] = p_value;
}

Variant MorphonConfigFile::get_value(const String &p_section, const String &p_key, const Variant &p_default) const
{
    if (!values.has(p_section) || !values[p_section].has(p_key))
    {
        ERR_FAIL_COND_V_MSG(p_default.get_type() == Variant::NIL, Variant(),
                            vformat("Couldn't find the given section \"%s\" and key \"%s\", and no default was given.", p_section, p_key));
        return p_default;
    }

    return values[p_section][p_key];
}

bool MorphonConfigFile::has_section(const String &p_section) const
{
    return values.has(p_section);
}

bool MorphonConfigFile::has_section_key(const String &p_section, const String &p_key) const
{
    if (!values.has(p_section))
    {
        return false;
    }
    return values[p_section].has(p_key);
}

PackedStringArray MorphonConfigFile::get_sections() const
{
    PackedStringArray array;
    for (const KeyValue<String, HashMap<String, Variant>> &E : values)
    {
        array.push_back(E.key);
    }

    return array;
}

PackedStringArray MorphonConfigFile::get_section_keys(const String &p_section) const
{
    PackedStringArray array;

    if (!values.has(p_section))
        return array;

    for (const KeyValue<String, Variant> &E : values[p_section])
    {
        array.push_back(E.key);
    }

    return array;
}

void MorphonConfigFile::erase_section(const String &p_section)
{
    ERR_FAIL_COND_MSG(!values.has(p_section), vformat("Cannot erase nonexistent section \"%s\".", p_section));
    values.erase(p_section);
}

void MorphonConfigFile::erase_section_key(const String &p_section, const String &p_key)
{
    ERR_FAIL_COND_MSG(!values.has(p_section), vformat("Cannot erase key \"%s\" from nonexistent section \"%s\".", p_key, p_section));
    ERR_FAIL_COND_MSG(!values[p_section].has(p_key), vformat("Cannot erase nonexistent key \"%s\" from section \"%s\".", p_key, p_section));

    values[p_section].erase(p_key);
    if (values[p_section].is_empty())
    {
        values.erase(p_section);
    }
}

void MorphonConfigFile::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_value", "section", "key", "value"), &MorphonConfigFile::set_value);
    ClassDB::bind_method(D_METHOD("get_value", "section", "key", "default"), &MorphonConfigFile::get_value, DEFVAL(Variant()));

    ClassDB::bind_method(D_METHOD("has_section", "section"), &MorphonConfigFile::has_section);
    ClassDB::bind_method(D_METHOD("has_section_key", "section", "key"), &MorphonConfigFile::has_section_key);

    ClassDB::bind_method(D_METHOD("get_sections"), &MorphonConfigFile::get_sections);
    ClassDB::bind_method(D_METHOD("get_section_keys", "section"), &MorphonConfigFile::get_section_keys);

    ClassDB::bind_method(D_METHOD("erase_section", "section"), &MorphonConfigFile::erase_section);
    ClassDB::bind_method(D_METHOD("erase_section_key", "section", "key"), &MorphonConfigFile::erase_section_key);

    /*ClassDB::bind_method(D_METHOD("load", "path"), &MorphonConfigFile::load);
    ClassDB::bind_method(D_METHOD("parse", "data"), &MorphonConfigFile::parse);
    ClassDB::bind_method(D_METHOD("save", "path"), &MorphonConfigFile::save);

    ClassDB::bind_method(D_METHOD("encode_to_text"), &MorphonConfigFile::encode_to_text);

    ClassDB::bind_method(D_METHOD("load_encrypted", "path", "key"), &MorphonConfigFile::load_encrypted);
    ClassDB::bind_method(D_METHOD("load_encrypted_pass", "path", "password"), &MorphonConfigFile::load_encrypted_pass);

    ClassDB::bind_method(D_METHOD("save_encrypted", "path", "key"), &MorphonConfigFile::save_encrypted);
    ClassDB::bind_method(D_METHOD("save_encrypted_pass", "path", "password"), &MorphonConfigFile::save_encrypted_pass);

    ClassDB::bind_method(D_METHOD("clear"), &MorphonConfigFile::clear);*/
}
