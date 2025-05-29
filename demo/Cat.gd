class_name Cat extends Animal

@export var color : Color
@export var byteArray : PackedByteArray

func Speak():
	print("meow ", age)
	
func _serialize() -> Dictionary:
	return {"color": color, "byteArray": byteArray}

func _deserialize(data : Dictionary):
	color = data["color"]
	byteArray = data["byteArray"]
	
	print("Deserialized byte array: ", byteArray)
