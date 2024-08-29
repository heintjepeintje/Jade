#pragma once

#include "./../Core.hpp"
#include <vector>
#include <initializer_list>

namespace Jade {

	enum struct InputElementType {
		None = 0,
		UInt32, Int32, Float32,
		Vector2, Vector3, Vector4,
	};
	
	struct InputElement {
		const char *Name;
		InputElementType Type;
	};
	
	class InputLayout {
	public:
		static InputLayout GetDefaultLayout();
		
	public:
		InputLayout() = default;
		InputLayout(const std::initializer_list<InputElement> &elements);
		~InputLayout() = default;
		
		inline size_t GetTotalSizeBytes() const { return m_TotalSizeBytes; }
		inline std::vector<InputElement> GetElements() const { return m_InputElements; }
		inline const InputElement &GetElement(uint32_t index) const { return m_InputElements[index]; }
		inline uint32_t GetElementCount() const { return (uint32_t)m_InputElements.size(); }
		inline bool IsEmpty() const { return m_TotalSizeBytes == 0; }
	private:
		size_t m_TotalSizeBytes = 0;
		std::vector<InputElement> m_InputElements;
	};

}