class_name Cat extends Animal

@export var color : Color

func Speak():
	print("meow")

func _serialize() -> Dictionary:
	var dict := super._serialize()
	dict["color"] = color.to_html()
	return dict

func _deserialize(data: Dictionary) -> void:
	super._deserialize(data)
	color = data["color"]
