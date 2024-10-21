///
/// @file BufferLayout.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Data/BufferLayout.h"

namespace KuchCraft {

	BufferElement::BufferElement(ShaderDataType type, const std::string& name)
		: Name(name), Type(type), Size(GetSize())
	{
	}

	uint32_t BufferElement::GetCount() const
	{
		switch (Type)
		{
			case ShaderDataType::Uint:   return 1;
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
		}

		return 0;
	}

	uint32_t BufferElement::GetSize() const
	{
		switch (Type)
		{
			case ShaderDataType::Uint:   return 1 * sizeof(uint32_t);
			case ShaderDataType::Int:    return 1 * sizeof(int32_t);
			case ShaderDataType::Float:  return 1 * sizeof(float);
			case ShaderDataType::Float2: return 2 * sizeof(float);
			case ShaderDataType::Float3: return 3 * sizeof(float);
			case ShaderDataType::Float4: return 4 * sizeof(float);
		}

		return 0;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& bufferElements)
		: m_BufferElements(bufferElements)
	{
		/// Calculate offsets and stride
		uint32_t offset = 0;
		for (auto& element : m_BufferElements)
		{
			/// Set the offset for the current element, update the offset for
			/// the next element and accumulate the total stride size
			element.Offset = offset;
			offset   += element.Size;
			m_Stride += element.Size;
		}

	}

}
