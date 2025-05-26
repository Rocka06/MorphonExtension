extends Node

var config := MorphonConfigFile.new()
@export var res : Dog
@export var dict : Dictionary[int, Dog]
@export var arr : Array[Animal]
@export var sf : SpriteFrames

func _ready():
	config.set_value("asd", "asd", "Hello")
	config.set_value("Pets", "Dog", res)
	config.set_value("Pets", "Dict", dict)
	config.set_value("Pets", "Arr", arr)
	config.set_value("Test", "SF", sf)
	config.save_encrypted_pass("user://save.json", "Test")
	
	config.clear()
	config.load_encrypted_pass("user://save.json", "Test")
	
	$AnimatedSprite2D.sprite_frames = config.get_value("Test", "SF")
	$AnimatedSprite2D.play("new_animation")
	
	var dog := config.get_value("Pets", "Dog") as Dog
	
	$Sprite2D.texture = dog.icon
	
	var loadedArr := config.get_value("Pets", "Arr") as Array
	for i in loadedArr:
		i.Speak();
