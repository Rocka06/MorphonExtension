using System;
using Godot;

public partial class MorphonConfigFile : RefCounted
{
    private Variant m_Config;

    public MorphonConfigFile()
    {
        m_Config = ClassDB.Instantiate("MorphonConfigFile");
    }

    public void SetValue(string section, string key, Variant value)
    {
        m_Config.As<GodotObject>().Call("set_value", section, key, value);
    }
    public Variant GetValue(string section, string key, Variant @default = default)
    {
        return m_Config.As<GodotObject>().Call("get_value", section, key, @default);
    }
    public bool HasSection(string section)
    {
        return m_Config.As<GodotObject>().Call("has_section", section).As<bool>();
    }
    public bool HasSectionKey(string section, string key)
    {
        return m_Config.As<GodotObject>().Call("has_section_key", section, key).As<bool>();
    }
    public string[] GetSections()
    {
        return m_Config.As<GodotObject>().Call("get_sections").As<string[]>();
    }
    public string[] GetSectionKeys(string section)
    {
        return m_Config.As<GodotObject>().Call("get_section_keys", section).As<string[]>();
    }
    public void EraseSection(string section)
    {
        m_Config.As<GodotObject>().Call("erase_section", section);
    }
    public void EraseSectionKey(string section, string key)
    {
        m_Config.As<GodotObject>().Call("erase_section_key", section);
    }
    public Error Load(string path)
    {
        return m_Config.As<GodotObject>().Call("load", path).As<Error>();
    }
    public Error Save(string path)
    {
        return m_Config.As<GodotObject>().Call("save", path).As<Error>();
    }
    public string EncodeToText()
    {
        return m_Config.As<GodotObject>().Call("encode_to_text").As<string>();
    }
    public Error LoadEncrypted(string path, byte[] key)
    {
        return m_Config.As<GodotObject>().Call("load_encrypted", path, key).As<Error>();
    }
    public Error LoadEncryptedPass(string path, string password)
    {
        return m_Config.As<GodotObject>().Call("load_encrypted_pass", path, password).As<Error>();
    }
    public Error SaveEncrypted(string path, byte[] key)
    {
        return m_Config.As<GodotObject>().Call("save_encrypted", path, key).As<Error>();
    }
    public Error SaveEncryptedPass(string path, string password)
    {
        return m_Config.As<GodotObject>().Call("save_encrypted_pass", path, password).As<Error>();
    }
    public void Clear()
    {
        m_Config = ClassDB.Instantiate("MorphonConfigFile");
        GC.Collect();
    }
}