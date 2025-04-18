#pragma once

import Easy.Core.Log;

#if EZ_DEBUG

#if defined(EZ_PLATFORM_WINDOWS)
#define HZ_DEBUGBREAK() __debugbreak()
#elif defined(EZ_PLATFORM_LINUX)
#include <signal.h>
#define EZ_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#define EZ_ENABLE_ASSERTS 1
#else
#define EZ_DEBUGBREAK()
#endif

#define EZ_CORE_TRACE(...)    ::Easy::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EZ_CORE_INFO(...)     ::Easy::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EZ_CORE_WARN(...)     ::Easy::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EZ_CORE_ERROR(...)    ::Easy::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EZ_CORE_CRITICAL(...) ::Easy::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define EZ_TRACE(...)         ::Easy::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EZ_INFO(...)          ::Easy::Log::GetClientLogger()->info(__VA_ARGS__)
#define EZ_WARN(...)          ::Easy::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EZ_ERROR(...)         ::Easy::Log::GetClientLogger()->error(__VA_ARGS__)
#define EZ_CRITICAL(...)      ::Easy::Log::GetClientLogger()->critical(__VA_ARGS__)

#if EZ_ENABLE_ASSERTS
#define EZ_CORE_ASSERT(x, ...) { if(!(x)) { EZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); EZ_DEBUGBREAK(); } }
#define EZ_ASSERT(x, ...) { if(!(x)) { EZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
#else
#define EZ_CORE_ASSERT(x, ...)
#define EZ_ASSERT(x, ...)
#endif
