using Godot;
using Godot.Collections;

[GlobalClass]
public partial class Car : Vehicle
{
    [Export] public Color color;

    public override void Deserialize(Dictionary<string, Variant> data)
    {
        base.Deserialize(data);
        color = data["color"].As<Color>();
    }

    public override void Serialize(out Dictionary<string, Variant> data)
    {
        base.Serialize(out data);
        data.Add("color", color.ToHtml());
    }
}