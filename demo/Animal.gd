class_name Animal extends SerializableResource

@export var name : String 
@export var age : int
@export var icon : Texture2D

func Speak():
	print("speak")

func _serialize() -> Dictionary:
	var dict : Dictionary
	dict["name"] = name
	dict["age"] = age
	dict["icon"] = icon
	
	return dict
	
func _deserialize(data: Dictionary):
	name = data["name"]
	age = data["age"]
	icon = data["icon"]
