#pragma once

#include <Jade/Core.hpp>

namespace Jade {

	namespace Native {
	
		void *_AllocBytes(size_t size);
		void *_ReAllocBytes(void *ptr, size_t size);
		void *_MoveBytes(void *src, void *dest, size_t size);
		void *_CopyBytes(const void *src, void *dest, size_t size);
		void _FillBytes(void *ptr, uint8_t byte, size_t size);
		bool _CompareBytes(const void *a, const void *b, size_t size);
		void _FreeBytes(void *ptr);
	
	}
	
}