class_name Cat extends Animal

@export var color : Color

func Speak():
	print("meow")

func serialize() -> Dictionary:
	var dict := super.serialize()
	dict["color"] = color.to_html()
	return dict

func deserialize(data: Dictionary) -> void:
	super.deserialize(data)
	color = data["color"]
