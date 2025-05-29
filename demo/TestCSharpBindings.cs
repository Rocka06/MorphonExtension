using System.Linq;
using Godot;
using Godot.Collections;

public partial class TestCSharpBindings : Node
{
    MorphonConfigFile config = new();

    [Export] Vehicle vehicle;
    [Export] Car[] cars;

    public override void _Ready()
    {
        MorphonSerializer.RegisterScriptByPath("Vehicle", "res://Vehicle.cs");
        MorphonSerializer.RegisterScriptByPath("Car", "res://Car.cs");

        config.SetValue("Test", "vehicle", vehicle);
        config.SetValue("Test", "Cars", cars);

        config.Save("user://csharpSave.json");
        config.Clear();
        config.Load("user://csharpSave.json");

        Car[] loadedCars = config.GetValue("Test", "Cars").As<Array<Car>>().ToArray<Car>();

        GD.Print(loadedCars[1].Brand);
    }
}
