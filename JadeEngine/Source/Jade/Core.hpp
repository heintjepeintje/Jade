#pragma once

#include "./Platform.hpp"
#include "./Log.hpp"
#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <string>
#include <memory>

#define JD_DEBUGBREAK() do { __asm__("int $0x03;"); } while (0)
#define JD_ASSERT(x, msg) if (!(x)) { JD_FATAL("Assertion failed: %s (%s, %u)", (msg), __BASE_FILE__, __LINE__); JD_DEBUGBREAK(); }
#define JD_PTR_OFFSET(ptr, offset) ((void*)(((uintptr_t)ptr) + ((uintptr_t)offset)))