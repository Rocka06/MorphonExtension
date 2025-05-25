extends Node

var config := MorphonConfigFile.new()
@export var res : Dog
@export var dict : Dictionary[int, Dog]
@export var arr : Array[Animal]

func _ready():
	config.set_value("asd", "asd", "Hello")
	config.set_value("Pets", "Dog", res)
	config.set_value("Pets", "Dict", dict)
	config.set_value("Pets", "Arr", arr)
	config.save("user://save.json")
	
	config.clear()
	config.load("user://save.json")
	
	var loadedArr := config.get_value("Pets", "Arr") as Array
	for i in loadedArr:
		i.Speak();
