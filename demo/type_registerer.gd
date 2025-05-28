extends Node

func _ready() -> void:
	MorphonSerializer.RegisterScript("Animal", ResourceLoader.load("res://Animal.gd"))
	MorphonSerializer.RegisterScript("Cat", ResourceLoader.load("res://Cat.gd"))
	MorphonSerializer.RegisterScript("Dog", ResourceLoader.load("res://Dog.gd"))
	
	MorphonSerializer.RegisterScript("Vehicle", ResourceLoader.load("res://Vehicle.cs"))
	MorphonSerializer.RegisterScript("Car", ResourceLoader.load("res://Car.cs"))
	
