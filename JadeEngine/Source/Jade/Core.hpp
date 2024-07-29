#pragma once

#include "./Platform.hpp"
#include "./Log.hpp"
#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <string>
#include <cstring>
#include <memory>

#ifdef _DEBUG
#	ifdef _WIN32
#		include <debugapi.h>
#		define JD_DEBUGBREAK() do { DebugBreak(); } while (0)
#	endif
#else
#	define JD_DEBUGBREAK()
#endif

#define JD_ASSERT(x, ...) if (!(x)) { JD_FATAL(__VA_ARGS__); JD_DEBUGBREAK(); }
#define JD_PTR_OFFSET(ptr, offset) ((void*)(((uintptr_t)ptr) + ((uintptr_t)offset)))
#define JD_ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

namespace Jade {

	template<typename _Type>
	using Ref = std::shared_ptr<_Type>;
	
	template<typename _Type, typename ..._Args>
	Ref<_Type> MakeRef(_Args&& ...args) {
		return std::make_shared<_Type>(std::forward<_Args>(args)...);
	}
	
	template<typename _CastType, typename _Type>
	Ref<_CastType> CastRef(const Ref<_Type> &ref) {
		return std::dynamic_pointer_cast<_CastType>(ref);
	}

}