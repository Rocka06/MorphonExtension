class_name Dog extends Animal

@export var type : String

func Speak():
	print("bark")

func _serialize() -> Dictionary:
	var dict := super._serialize()
	dict["type"] = type
	return dict

func _deserialize(data: Dictionary) -> void:
	super._deserialize(data)
	type = data["type"]
