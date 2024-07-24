#pragma once

#include "./../Core.hpp"

namespace Jade {

	void *AllocBytes(size_t size);
	void *ReAllocBytes(void *ptr, size_t size);
	void *MoveBytes(void *src, void *dest, size_t size);
	void *CopyBytes(const void *src, void *dest, size_t size);
	void FillBytes(void *ptr, uint8_t byte, size_t size);
	bool CompareBytes(const void *a, const void *b, size_t size);
	void FreeBytes(void *ptr);
	
	template<typename _Type>
	_Type *Alloc(uint32_t count) { return (_Type*)AllocBytes(count * sizeof(_Type)); }
	
	template<typename _Type>
	_Type *ReAlloc(_Type *ptr, uint32_t count) { return (_Type*)ReAllocBytes((void*)ptr, count * sizeof(_Type)); }
	
	template<typename _Type>
	_Type *MoveMemory(_Type *src, _Type *dest, uint32_t count) { return (_Type*)MoveBytes((void*)src, (void*)dest, count * sizeof(_Type)); }
	
	template<typename _Type>
	_Type *CopyMemory(const _Type *src, _Type *dest, uint32_t count) { return (_Type*)CopyBytes((const void*)src, (void*)dest, count * sizeof(_Type)); }
	
	template<typename _Type>
	bool CompareMemory(const _Type *a, const _Type *b, uint32_t count) { return (_Type*)CompareBytes((const void*)a, (const void *)b, count * sizeof(_Type)); }

	template<typename _Type>
	void FillMemory(_Type *ptr, uint8_t byte, uint32_t count) { FillBytes((void*)ptr, byte, count); }

	template<typename _Type>
	void Free(_Type *ptr) { FreeBytes((void*)ptr); }
	
	class Buffer {
	public:
		Buffer();
		Buffer(size_t size);
		Buffer(const void *data, size_t size);
		Buffer(const Buffer &buffer);
		Buffer(Buffer &&buffer);
		~Buffer();
		
		void Resize(size_t size);
		void WriteBytes(size_t offset, const void *data, size_t size);
		void ReadBytes(size_t offset, void *data, size_t size) const;
		
		template<typename _Type>
		inline void Write(size_t offset, const _Type *data, uint32_t count = 1) { WriteBytes(offset, (const void*)data, count * sizeof(_Type)); }
		
		template<typename _Type>
		inline void Read(size_t offset, _Type *data, uint32_t count = 1) { ReadBytes(offset, (void*)data, count * sizeof(_Type)); }
		
		inline void *GetData() { return m_Data; }
		inline const void *GetData() const { return m_Data; }
		inline size_t GetSize() const { return m_Size; }
	private:
		void *m_Data;
		size_t m_Size;
	};

}