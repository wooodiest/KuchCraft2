///
/// @file VertexArray.cpp
/// 
/// @author Michal Kuchnicki
/// 
/// 

#include "kcpch.h"
#include "Graphics/Data/VertexArray.h"

#include <glad/glad.h>

namespace KuchCraft {

	/// Converts a ShaderDataType to the corresponding OpenGL base type.
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Uint:   return GL_UNSIGNED_INT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
		}

		return 0;
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Create()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void VertexArray::SetVertexBuffer(const VertexBuffer& vertexBuffer)
	{
		Bind();
		vertexBuffer.Bind();

		uint32_t index = 0;
		const auto& bufferLayout = vertexBuffer.GetBufferLayout();
		for (const auto& element : bufferLayout)
		{
			/// Setup the layout based on the ShaderDataType.
			switch (element.Type)
			{
				case ShaderDataType::Uint:
				case ShaderDataType::Int:
				{
					/// For integer attributes, use glVertexAttribIPointer.
					glVertexAttribIPointer(
						index,
						element.GetCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						bufferLayout.GetStride(),
						(const void*)(size_t)element.Offset
					);
					glEnableVertexAttribArray(index);
					index++;
					break;
				}
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					/// For floating-point attributes, use glVertexAttribPointer.
					glVertexAttribPointer(
						index,
						element.GetCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						GL_FALSE,
						bufferLayout.GetStride(),
						(const void*)(size_t)element.Offset
					);

					glEnableVertexAttribArray(index);
					index++;
					break;
				}
			}
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}
