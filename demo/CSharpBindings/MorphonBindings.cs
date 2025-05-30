using System;
using System.Reflection;
using Godot;

public partial class MorphonConfigFile : RefCounted
{
    private Variant m_Config;
    private ConfigFile asd;

    public MorphonConfigFile()
    {
        //Register C# types
        Type baseType = typeof(Resource);
        foreach (Type type in Assembly.GetExecutingAssembly().GetTypes())
        {
            if (!baseType.IsAssignableFrom(type) || type.IsAbstract) continue;

            Resource res = (Resource)Activator.CreateInstance(type);
            Script script = res.GetScript().As<Script>();
            MorphonSerializer.RegisterScriptByPath(type.FullName, script.ResourcePath);
        }

        GC.Collect();

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
    public T GetValue<[MustBeVariant] T>(string section, string key, T @default = default)
    {
        return m_Config.As<GodotObject>().Call("get_value", section, key, Variant.From(@default)).As<T>();
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
    public Error Parse(string data)
    {
        return m_Config.As<GodotObject>().Call("parse", data).As<Error>();
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

public partial class MorphonSerializer : GodotObject
{
    public static void RegisterScript(string name, Script script)
    {
        ClassDB.ClassCallStatic("MorphonSerializer", "register_script", name, script);
    }

    public static void RegisterScriptByPath(string name, string scriptPath)
    {
        ClassDB.ClassCallStatic("MorphonSerializer", "register_script_by_path", name, scriptPath);
    }
}