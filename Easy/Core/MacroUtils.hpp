#pragma once

import Easy.Core.Basic;
import Easy.Core.Log;

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
#define UNIQUE_REF(base) CONCAT(base, __LINE__)

#if EZ_DEBUG

#if defined(EZ_PLATFORM_WINDOWS)
#define EZ_DEBUGBREAK() __debugbreak()
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

#ifdef __clang__
#define EZ_CORE_ASSERT_MESSAGE_INTERNAL(...) EZ_CORE_ERROR("Assertion Failed", ##__VA_ARGS__)
#define EZ_ASSERT_MESSAGE_INTERNAL(...)      EZ_ERROR("Assertion Failed", ##__VA_ARGS__)
#else
#define EZ_CORE_ASSERT_MESSAGE_INTERNAL(...) EZ_CORE_ERROR("Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
#define EZ_ASSERT_MESSAGE_INTERNAL(...)      EZ_ERROR("Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
#endif


#ifdef EZ_ENABLE_ASSERTS
#define EZ_CORE_ASSERT(x, ...) \
do { if (!(x)) { EZ_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); EZ_DEBUGBREAK(); } } while(0)
#define EZ_ASSERT(x, ...) \
do { if (!(x)) { EZ_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); EZ_DEBUGBREAK(); } } while(0)
#else
#define EZ_CORE_ASSERT(x, ...)
#define EZ_ASSERT(x, ...)
#endif

#define EZ_ENABLE_PROFILING 1

#if EZ_ENABLE_PROFILING
#define EZ_PROFILE_FRAME_START() Easy::StartProfiling()
#define EZ_PROFILE_FRAME_END() Easy::EndProfiling()
#define EZ_PROFILE_FUNCTION() \
constexpr static auto UNIQUE_NAME(loc) = std::source_location::current(); const auto UNIQUE_NAME(profile_guard) = []() { constexpr static const auto& loc = UNIQUE_REF(loc);\
constexpr static auto line_str = SizeTToString<loc.line()>;\
constexpr static auto line_sv = std::string_view(line_str.data(), line_str.size());\
return Easy::Profile([](std::chrono::microseconds ms) { \
g_ProfileInfos.push_back({ \
{"Function: ", loc.function_name(), ", Line: ", line_sv}, \
ms \
}); \
}); \
}()

#define EZ_PROFILE_SCOPE()\
constexpr static auto UNIQUE_NAME(loc) = std::source_location::current(); const auto UNIQUE_NAME(profile_guard) = []() { constexpr static const auto& loc = UNIQUE_REF(loc);\
constexpr static auto line_str = SizeTToString<loc.line()>;\
constexpr static auto line_sv = std::string_view(line_str.data(), line_str.size());\
return Easy::Profile([](std::chrono::microseconds ms) { \
g_ProfileInfos.push_back({ \
{"File: ", loc.file_name(), ", Line:", line_sv}, \
ms \
}); \
}); \
}()

#define EZ_PROFILE_NAME(name)\
constexpr static auto CONCAT(loc, name) = std::source_location::current(); const auto UNIQUE_NAME(profile_guard) = []() { constexpr static const auto& loc = CONCAT(loc, name);\
constexpr static auto line_str = SizeTToString<loc.line()>;\
constexpr static auto line_sv = std::string_view(line_str.data(), line_str.size());\
return Easy::Profile([](std::chrono::microseconds ms) { \
g_ProfileInfos.push_back({ \
{"Name: ", #name, ", Line: ", line_sv}, \
ms \
}); \
}); \
}()

#else
#define EZ_PROFILE_FRAME_START()
#define EZ_PROFILE_FRAME_END()
#define EZ_PROFILE_FUNCTION()
#endif