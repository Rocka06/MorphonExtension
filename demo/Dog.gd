class_name Dog extends Animal

@export var type : String

func Speak():
	print("bark")

func serialize() -> Dictionary:
	var dict := super.serialize()
	dict["type"] = type
	return dict

func deserialize(data: Dictionary) -> void:
	super.deserialize(data)
	type = data["type"]
