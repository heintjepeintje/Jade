#include "./InputLayout.hpp"

namespace Jade {

	size_t GetInputElementTypeSize(InputElementType type) {
		switch (type) {
			case InputElementType::Vector2: return 2 * sizeof(float);
			case InputElementType::Vector3: return 3 * sizeof(float);
			case InputElementType::Vector4: return 4 * sizeof(float);
			default: return 0;
		}
	}
	
	InputLayout::InputLayout(const std::initializer_list<InputElement> &elements) {
		for (InputElement e : elements) {
			m_InputElements.push_back(e);
			m_TotalSizeBytes += GetInputElementTypeSize(e.Type);
		} 
	}
	
	InputLayout InputLayout::GetDefaultLayout() {
		InputLayout layout = {
			{ "Position", InputElementType::Vector3 },
			{ "UV", InputElementType::Vector2 },
			{ "Color", InputElementType::Vector4 },
		};
		
		return layout;
	}

}