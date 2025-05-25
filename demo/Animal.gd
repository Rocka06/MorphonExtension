class_name Animal extends SerializableResource

@export var name : String 
@export var age : int

func Speak():
	print("speak")

func _serialize() -> Dictionary:
	var dict : Dictionary
	dict["name"] = name
	dict["age"] = age
	
	return dict
	
func _deserialize(data: Dictionary):
	name = data["name"]
	age = data["age"]
