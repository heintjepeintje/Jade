#pragma once

#if defined(_WIN32)
#	define JD_PLATFORM_WINDOWS 1
#elif defined(__unix__)
#	define JD_PLATFORM_UNIX 1
#elif defined(__linux__)
#	define JD_PLATFORM_LINUX 1
#elif defined(__APPLE__)
#	include "TargetConditionals.h"
#	if defined(TARGET_OS_IPHONE)
#		define JD_PLATFORM_IPHONE 1
#	elif defined(__MACH__)
#		define JD_PLATFORM_MACOS 1
#	endif
#elif defined(__ANDROID__)
#	define JD_PLATFORM_ANDROID 1
#endif

#if defined(__GNUC__)
#	define JD_COMPILER_GCC 1
#	define JD_UNUSED __attribute__((unused))
#	define JD_NORETURNFN __attribute__((noreturn))
#	define JD_CONSTFN __attribute__((const))
#	define JD_STDCALL __attribute__((__stdcall__))
#	define JD_CDECL __attribute__((__cdecl__))
#	define JD_FASTCALL __attribute__((__fastcall__))
#	define JD_IMPORT __attribute__((dllimport))
#	define JD_EXPORT __attribute__((dllexport))
#elif defined(__clang__)
#	define JD_COMPILER_CLANG 1
#	error Jade only supports g++ for now
#elif defined(_MSC_VER)
#	define JD_COMPILER_MSVC 1
#	error Jade only supports g++ for now
#endif

#ifndef __cplusplus
#	error Jade is written in C++
#endif

// TODO: Detect platform architecture (32-bit or 64-bit)