///
/// @file IndexBuffer.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Data/IndexBuffer.h"

#include <glad/glad.h>

namespace KuchCraft {

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::Create(IndexBufferDataUsage usage, uint32_t count, uint32_t* data)
	{
		if (m_RendererID)
			glDeleteBuffers(1, &m_RendererID);

		m_Count = count;
		m_Usage = usage;

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(uint32_t), data, usage == IndexBufferDataUsage::STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

		m_Info = "Index Buffer -> id: "      + std::to_string(m_RendererID) +
							   ", indices: " + std::to_string(m_Count) +
							   ", usage: "   + (usage == IndexBufferDataUsage::STATIC ? "static" : "dynamic");
	}

	void IndexBuffer::SetData(uint32_t* data, uint32_t count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}