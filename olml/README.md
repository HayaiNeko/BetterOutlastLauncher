# Outlast Mod Loader

The **Outlast Mod Loader** (OLML for short) is a piece of software used within the **Better Outlast Launcher** to automatically load mods from the `mods` directory. In order for a mod to be compatible with this loader, it has to implement a special `OutlastMod` interface. The loader provides a definition for that interface along with other useful utilities that might help you create your own mod.

## Creating your own mod

In order to create a mod you will need an empty C++ DLL project.

The first step is to copy `modloaderapi.hpp` and `modloaderapi.cpp` into your project. Make sure you read the contents of the header file to get acquainted with the utilities available to you during mod development.

After that, you can create a new class for your mod that inherits from `OutlastMod` and implements all required functions.

```cpp
#include <modloaderapi.hpp>

class BestModEver : public OutlastMod {
public:
  const char* GetName() override { return "Best mod ever!"; }

  const char* GetAuthor() override { return "Your name"; }

  void Init() override { /* Initialization stuff */ }
}
```

Now you will have to change one thing in `modloaderapi.cpp`. Open the file and change line 6 and 7 to use your mod class.

```cpp
#include <bestmodever.hpp>
using OutlastModType = BestModEver;
```

This file also contains an empty entry point for your DLL, as well as an exported function that allows the mod loader to access the interface.

At this point you should be able to compile and load your mod into the game by putting it inside the mods folder. Make sure to link your program against `olml.x64.dll` or `olml.x86.dll` depending on the version of the game you are developing for.

*Note that the current version of the launcher (v1.2.1) does not implement the latest version of the mod loader. As such, the example mod shown in this article will not work, please wait for the next update before testing.*