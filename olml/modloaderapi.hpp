#pragma once

// Outlast Mod Loader API
// This file includes utilities you should use in your mod
// The following code was written for C++23

#include <cstdint>
#include <string>
#include <format>

#ifdef IS_OLML
  #define OLML_API __declspec(dllexport)
#else
  #define OLML_API __declspec(dllimport)
#endif

// Architecture specific defines useful for developing mods compatible with both 32 and 64 bit Outlast
// Use IS_WIN32() and IS_WIN64() to check which architecture you're currently compiling to
#if defined(_WIN64)
  #define IS_WIN32() false
  #define IS_WIN64() true
#elif defined(_WIN32)
  #define IS_WIN32() true
  #define IS_WIN64() false
#endif

// Use WIN32_ARGS() to write 32 bit only code
// Use WIN32_WIN64() to switch between 32 and 64 bit code depending on the architecture you're compiling to
// Use FASTCALL_THIS() in __fastcall hooks in order to automatically add an extra argument on 32 bit
#if IS_WIN64()
  #define WIN32_ARGS(...)
  #define WIN32_WIN64(win32, win64) win64
  #define FASTCALL_THIS(_this) _this
#elif IS_WIN32()
  #define WIN32_ARGS(...) __VA_ARGS__
  #define WIN32_WIN64(win32, win64) win32
  #define FASTCALL_THIS(_this) _this, void*
#endif

// These calling convention defines become empty on 64 bit
#define THISCALL_CONV WIN32_ARGS(__thiscall)
#define FASTCALL_CONV WIN32_ARGS(__fastcall)
#define STDCALL_CONV  WIN32_ARGS(__stdcall)
#define CDECL_CONV    WIN32_ARGS(__cdecl)

// OutlastMod is the base class of every mod
// Your mod class has to implement these functions in order to be loaded by the mod loader
class OutlastMod {
public:
  virtual ~OutlastMod() = default;
  virtual const char* GetName() = 0;
  virtual const char* GetAuthor() = 0;
  virtual void Init() = 0;
};

// Logging utilities
// The log level will be displayed before the message in the log file
enum class LogLevel : uint8_t {
  kInfo,
  kWarning,
  kError
};

// Use the Logger class to write messages into the log file
class Logger {
public:
  template <typename... FormatArgs>
  void Info(const std::string& message, FormatArgs&&... args) {
    Log(std::vformat(message, std::make_format_args(args...)), LogLevel::kInfo);
  }

  template <typename... FormatArgs>
  void Warning(const std::string& message, FormatArgs&&... args) {
    Log(std::vformat(message, std::make_format_args(args...)), LogLevel::kWarning);
  }

  template <typename... FormatArgs>
  void Error(const std::string& message, FormatArgs&&... args) {
    Log(std::vformat(message, std::make_format_args(args...)), LogLevel::kError);
  }

  // Use this function to get a Logger through which you can write messages
  OLML_API static Logger* GetInstance();

private:
  OLML_API void Log(const std::string& message, LogLevel level);
};

// Hooking utilities
// Internal class for creating hooks, you can use this if you don't want to manage your hooks using the template class below
class HookImpl {
public:
  OLML_API static void CreateHook(void* target, void* detour, void** trampoline);
};

// Use the Hook class to create and manage hooks
// You can specify the trampoline type either when instantiating the class or when calling the GetTrampoline() method
template <typename TrampolineType = void*>
class Hook {
public:
  // Use this function to create the hook
  template <typename TargetType, typename DetourType>
  void Install(TargetType target, DetourType detour) {
    HookImpl::CreateHook((void*)target, (void*)detour, &trampoline_);
  }

  // Use this function to get the trampoline
  template <typename CastedType = TrampolineType>
  CastedType GetTrampoline() const { return (CastedType)trampoline_; }

private:
  void* trampoline_ = nullptr;
};