using System.Linq;
using Godot;
using Godot.Collections;

public partial class TestCSharpBindings : Node
{
    MorphonConfigFile config = new();

    [Export] Vehicle vehicle;
    [Export] Car[] cars;

    [Export] Label label;

    public override void _Ready()
    {
        config.SetValue("Test", "vehicle", vehicle);
        config.SetValue("Test", "Cars", cars);

        config.Save("user://csharpSave.json");
        config.Clear();
        config.Load("user://csharpSave.json");

        Car[] loadedCars = config.GetValue("Test", "Cars").As<Array<Car>>().ToArray();

        label.Text = loadedCars[1].Brand;
    }
}
