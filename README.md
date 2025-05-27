# Morphon GDExtension

Morphon is a simple library for saving and loading game data, that works just like the built-in ConfigFile class with one exception.
You can't inject malicious code into a save file!

---

## Features

- `SerializableResource`: A virtual base `Resource` class with a `_serialize()` and `_deserialize()` function.
- `MorphonConfigFile`: JSON-based save system, that works similarly to the built-in `ConfigFile`.
- It can be used with GDScript, or if you want, you can also download the C# bindings, to use the extension with C#.

## 🔧 Build Instructions

### Prerequisites

- Godot 4.x (matching version)
- Python 3.x
- SCons
- C++ compiler (e.g., MSVC, Clang, or GCC)

### Building

1. Clone and prepare submodules:

  ```bash
  git clone --recursive https://github.com/Rocka06/MorphonExtension.git
  cd MorphonExtension
  ```

2. Checkout the matching Godot version branch:

  ```bash
  cd godot-cpp
  git checkout <godot-version-branch>  # e.g., 4.4
  ```

3. Build the extension:
  Go back to the root directory and type `scons` into the terminal.

---

## Usage

### GDScript

```gdscript
var config := MorphonConfigFile.new()
config.load("user://save.json")
print(config.get_value("section", "key"))
```

### C#

```csharp
MorphonConfigFile config = new MorphonConfigFile();
config.Load("user://save.json");
Variant value = config.GetValue("section", "key");
```
