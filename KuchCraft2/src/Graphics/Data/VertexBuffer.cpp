///
/// @file VertexBuffer.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Data/VertexBuffer.h"

#include <glad/glad.h>

namespace KuchCraft {

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Create(VertexBufferDataUsage usage, uint32_t size, const void* data)
	{
		if (m_RendererID)
			glDeleteBuffers(1, &m_RendererID);

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage == VertexBufferDataUsage::STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::SetData(uint32_t size, const void* data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
