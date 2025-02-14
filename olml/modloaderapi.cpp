#include <modloaderapi.hpp>

#include <windows.h>

// Include your mod class and replace MyMod with it
#include <mymod.hpp>
using OutlastModType = MyMod;

// Export the mod interface so the mod loader can use it
extern "C" __declspec(dllexport) OutlastMod* GetOutlastModInterface() {
  static auto inst = new OutlastModType();
  return inst;
}

// The entry point of your mod should be empty, use the Init method inside your mod class instead
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved) {
  return TRUE;
}
