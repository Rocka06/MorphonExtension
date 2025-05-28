#include "MorphonConfigFile.h"

void MorphonConfigFile::set_value(const String &p_section, const String &p_key, const Variant &p_value)
{
    if (p_value.get_type() == Variant::NIL)
    { // Erase key.
        if (!m_Values.has(p_section))
        {
            return;
        }

        m_Values[p_section].erase(p_key);
        if (m_Values[p_section].is_empty())
        {
            m_Values.erase(p_section);
        }

        return;
    }

    if (!m_Values.has(p_section))
    {
        // Insert section-less keys at the beginning.
        m_Values.insert(p_section, HashMap<String, Variant>(), p_section.is_empty());
    }

    m_Values[p_section][p_key] = p_value;
}

Variant MorphonConfigFile::get_value(const String &p_section, const String &p_key, const Variant &p_default) const
{
    if (!m_Values.has(p_section) || !m_Values[p_section].has(p_key))
    {
        ERR_FAIL_COND_V_MSG(p_default.get_type() == Variant::NIL, Variant(),
                            vformat("Couldn't find the given section \"%s\" and key \"%s\", and no default was given.", p_section, p_key));
        return p_default;
    }

    return m_Values[p_section][p_key];
}

bool MorphonConfigFile::has_section(const String &p_section) const
{
    return m_Values.has(p_section);
}

bool MorphonConfigFile::has_section_key(const String &p_section, const String &p_key) const
{
    if (!m_Values.has(p_section))
    {
        return false;
    }
    return m_Values[p_section].has(p_key);
}

PackedStringArray MorphonConfigFile::get_sections() const
{
    PackedStringArray array;
    for (const KeyValue<String, HashMap<String, Variant>> &E : m_Values)
    {
        array.push_back(E.key);
    }

    return array;
}

PackedStringArray MorphonConfigFile::get_section_keys(const String &p_section) const
{
    PackedStringArray array;

    if (!m_Values.has(p_section))
        return array;

    for (const KeyValue<String, Variant> &E : m_Values[p_section])
    {
        array.push_back(E.key);
    }

    return array;
}

void MorphonConfigFile::erase_section(const String &p_section)
{
    ERR_FAIL_COND_MSG(!m_Values.has(p_section), vformat("Cannot erase nonexistent section \"%s\".", p_section));
    m_Values.erase(p_section);
}

void MorphonConfigFile::erase_section_key(const String &p_section, const String &p_key)
{
    ERR_FAIL_COND_MSG(!m_Values.has(p_section), vformat("Cannot erase key \"%s\" from nonexistent section \"%s\".", p_key, p_section));
    ERR_FAIL_COND_MSG(!m_Values[p_section].has(p_key), vformat("Cannot erase nonexistent key \"%s\" from section \"%s\".", p_key, p_section));

    m_Values[p_section].erase(p_key);
    if (m_Values[p_section].is_empty())
    {
        m_Values.erase(p_section);
    }
}

Error MorphonConfigFile::save(const String &p_path) const
{
    Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::WRITE);
    if (f.is_null())
        return f->get_open_error();

    f->store_string(encode_to_text());
    f->close();

    return OK;
}

Error MorphonConfigFile::load(const String &p_path)
{
    Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ);

    if (f.is_null())
        return f->get_open_error();

    String stringData = f->get_as_text();
    f->close();
    clear();

    return parse(stringData);
}

String MorphonConfigFile::encode_to_text() const
{
    Dictionary dict;
    for (auto i : m_Values)
    {
        Dictionary nestedDict;
        for (auto j : i.value)
        {
            nestedDict[j.key] = MorphonSerializer::SerializeRecursive(j.value);
        }

        dict[i.key] = nestedDict;
    }

    return JSON::stringify(dict);
}
Error MorphonConfigFile::parse(const String &data)
{
    Ref<JSON> json;
    json.instantiate();
    Error err = json->parse(data);

    if (err != OK)
    {
        print_line("JSON Parse Error: ", err);
        return err;
    }

    Variant jsonVariant = json->get_data();

    if (jsonVariant.get_type() != Variant::DICTIONARY)
        return ERR_INVALID_DATA;

    Dictionary dict = jsonVariant;
    Array keys = dict.keys();

    clear();

    for (int i = 0; i < keys.size(); i++)
    {
        Variant key = keys[i];
        Variant value = dict[key];

        if (value.get_type() != Variant::DICTIONARY)
            return ERR_INVALID_DATA;

        Dictionary valueDict = value;
        Array valueKeys = valueDict.keys();

        for (int j = 0; j < valueKeys.size(); j++)
        {
            Variant key1 = valueKeys[j];
            Variant value1 = valueDict[key1];

            m_Values[key][key1] = MorphonSerializer::DeserializeRecursive(value1);
        }
    }

    return OK;
}

Error MorphonConfigFile::load_encrypted(const String &p_path, const PackedByteArray &p_key)
{
    Ref<FileAccess> f = FileAccess::open_encrypted(p_path, FileAccess::READ, p_key);

    if (f.is_null())
        return f->get_open_error();

    String stringData = f->get_as_text();
    f->close();
    clear();

    return parse(stringData);
}
Error MorphonConfigFile::load_encrypted_pass(const String &p_path, const String &p_password)
{
    Ref<FileAccess> f = FileAccess::open_encrypted_with_pass(p_path, FileAccess::READ, p_password);

    if (f.is_null())
        return f->get_open_error();

    String stringData = f->get_as_text();
    f->close();
    clear();

    return parse(stringData);
}
Error MorphonConfigFile::save_encrypted(const String &p_path, const PackedByteArray &p_key) const
{
    Ref<FileAccess> f = FileAccess::open_encrypted(p_path, FileAccess::WRITE, p_key);
    if (f.is_null())
        return f->get_open_error();

    f->store_string(encode_to_text());
    f->close();

    return OK;
}
Error MorphonConfigFile::save_encrypted_pass(const String &p_path, const String &p_password) const
{
    Ref<FileAccess> f = FileAccess::open_encrypted_with_pass(p_path, FileAccess::WRITE, p_password);
    if (f.is_null())
        return f->get_open_error();

    f->store_string(encode_to_text());
    f->close();

    return OK;
}

void MorphonConfigFile::clear()
{
    m_Values.clear();
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

    ClassDB::bind_method(D_METHOD("load", "path"), &MorphonConfigFile::load);
    ClassDB::bind_method(D_METHOD("save", "path"), &MorphonConfigFile::save);

    ClassDB::bind_method(D_METHOD("encode_to_text"), &MorphonConfigFile::encode_to_text);
    ClassDB::bind_method(D_METHOD("parse", "data"), &MorphonConfigFile::parse);

    ClassDB::bind_method(D_METHOD("load_encrypted", "path", "key"), &MorphonConfigFile::load_encrypted);
    ClassDB::bind_method(D_METHOD("load_encrypted_pass", "path", "password"), &MorphonConfigFile::load_encrypted_pass);

    ClassDB::bind_method(D_METHOD("save_encrypted", "path", "key"), &MorphonConfigFile::save_encrypted);
    ClassDB::bind_method(D_METHOD("save_encrypted_pass", "path", "password"), &MorphonConfigFile::save_encrypted_pass);

    ClassDB::bind_method(D_METHOD("clear"), &MorphonConfigFile::clear);
}
