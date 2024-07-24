#include "./Memory.hpp"

#include "./../Platform/NativeMemory.hpp"

namespace Jade {

	void *AllocBytes(size_t size) { return Native::_AllocBytes(size); }
	void *ReAllocBytes(void *ptr, size_t size) { return Native::_ReAllocBytes(ptr, size); }
	void *MoveBytes(void *src, void *dest, size_t size) { return Native::_MoveBytes(src, dest, size); }
	void *CopyBytes(const void *src, void *dest, size_t size) { return Native::_CopyBytes(src, dest, size); }
	void FillBytes(void *ptr, uint8_t byte, size_t size) { Native::_FillBytes(ptr, byte, size); }
	bool CompareBytes(const void *a, const void *b, size_t size) { return Native::_CompareBytes(a, b, size); }
	void FreeBytes(void *ptr) { Native::_FreeBytes(ptr); }
	
	Buffer::Buffer() {
		m_Data = nullptr;
		m_Size = 0;
	}
	
	Buffer::Buffer(size_t size) {
		m_Size = size;
		m_Data = AllocBytes(m_Size);
	}
	
	Buffer::Buffer(const void *data, size_t size) {
		m_Data = AllocBytes(size);
		CopyBytes(data, m_Data, size);
		m_Size = size;
	}
	
	Buffer::Buffer(const Buffer &other) {
		m_Data = AllocBytes(other.m_Size);
		CopyBytes(other.m_Data, m_Data, other.m_Size);
		m_Size = other.m_Size;
	}
	
	Buffer::Buffer(Buffer &&other) {
		m_Data = other.m_Data;
		other.m_Data = nullptr;
		m_Size = other.m_Size;
	}
	
	Buffer::~Buffer() {
		if (m_Data != nullptr && m_Size != 0) {
			FreeBytes(m_Data);
		}
	}
	
	void Buffer::Resize(size_t size) {
		if (m_Size >= size) return;
		m_Data = ReAllocBytes(m_Data, size);
		m_Size = size;
	}
	
	void Buffer::WriteBytes(size_t offset, const void *data, size_t size) {
		CopyBytes(data, JD_PTR_OFFSET(m_Data, offset), size);
	}
	
	void Buffer::ReadBytes(size_t offset, void *data, size_t size) const {
		CopyBytes(JD_PTR_OFFSET(m_Data, offset), data, size);
	}
	
}