#ifdef _WIN32

#include "./../NativeMemory.hpp"

#include "./Windows.hpp"

namespace Jade {

	namespace Native {
	
		void *_AllocBytes(size_t size) {
			LPVOID ptr = HeapAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				size);
				
			JD_ASSERT(ptr != NULL, "Failed to allocate memory");
				
			return ptr;
		}
		
		void *_ReAllocBytes(void *ptr, size_t size) {
			LPVOID result = HeapReAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				ptr,
				size);
				
			JD_ASSERT(result != NULL, "Failed to re-allocate memory");
			
			return result;
		}
		
		void *_MoveBytes(void *src, void *dest, size_t size) {
			(void)RtlMoveMemory(dest, src, size);
			return dest;
		}
		
		void *_CopyBytes(const void *src, void *dest, size_t size) {
			(void)RtlCopyMemory(dest, src, size);
			return dest;
		}
		
		void _FillBytes(void *ptr, uint8_t byte, size_t size) {
			(void)RtlFillMemory(ptr, size, byte);
		} 
		
		bool _CompareBytes(const void *a, const void *b, size_t size) {
			return RtlEqualMemory(a, b, size);
		}
		
		void _FreeBytes(void *ptr) {
			WINBOOL result = HeapFree(
				GetProcessHeap(),
				0,
				ptr);
			
			JD_ASSERT(result, "Failed to free memory"); 
		}
	
	}
	
}

#endif // _WIN32