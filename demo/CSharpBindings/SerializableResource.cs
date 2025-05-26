using Godot;
using Godot.Collections;

public abstract partial class SerializableResource : Resource
{
    private Dictionary _serialize()
    {
        Serialize(out Dictionary<string, Variant> dict);
        Variant varDict = dict;

        return varDict.As<Dictionary>();
    }

    private void _deserialize(Dictionary data)
    {
        Variant varDict = data;

        Deserialize(varDict.As<Dictionary<string, Variant>>());
    }

    public abstract void Serialize(out Dictionary<string, Variant> data);
    public abstract void Deserialize(Dictionary<string, Variant> data);
}