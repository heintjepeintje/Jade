#pragma once

#include "./../Core.hpp"

namespace Jade {

	void *AllocBytes(size_t size);
	void *ReAllocBytes(void *ptr, size_t size);
	void FreeBytes(void *ptr);
	
	template<typename _Type>
	_Type *Alloc(uint32_t count) { return (_Type*)AllocBytes(count * sizeof(_Type)); }
	
	template<typename _Type>
	_Type *ReAlloc(_Type *ptr, uint32_t count) { return (_Type*)ReAllocBytes((void*)ptr, count * sizeof(_Type)); }

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
		
		template<typename _Type>
		inline _Type *As() { return (_Type*)m_Data; }
		
		template<typename _Type>
		inline const _Type *As() const { return (const _Type*)m_Data; }
		
		inline void *GetData() { return m_Data; }
		inline const void *GetData() const { return m_Data; }
		inline size_t GetSize() const { return m_Size; }
	private:
		void *m_Data;
		size_t m_Size;
	};

}