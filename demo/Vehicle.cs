using Godot;
using Godot.Collections;

[GlobalClass]
public partial class Vehicle : SerializableResource
{
    [Export] public string Brand;

    public override void Deserialize(Dictionary<string, Variant> data)
    {
        Brand = data["Brand"].As<string>();
    }

    public override void Serialize(out Dictionary<string, Variant> data)
    {
        data = new()
        {
            {"Brand", Brand }
        };
    }
}