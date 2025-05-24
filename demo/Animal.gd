class_name Animal extends SerializableResource

func serialize() -> Dictionary:
	print("hello")
	
	return {}
	
func deserialize(data: Dictionary):
	print("hello")
