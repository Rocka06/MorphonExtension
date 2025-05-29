using Godot;

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