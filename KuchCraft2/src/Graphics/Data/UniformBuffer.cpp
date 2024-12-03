#include "kcpch.h"
#include "Graphics/Data/UniformBuffer.h"

#include <glad/glad.h>

namespace KuchCraft {

	static inline uint32_t s_NextAvailableBinding = 0;
	static inline std::set<uint32_t> s_FreeBindings;

	static uint32_t AllocateBinding()
	{
		if (!s_FreeBindings.empty())
		{
			auto it = s_FreeBindings.begin();
			uint32_t binding = *it;
			s_FreeBindings.erase(it);
			return binding;
		}

		return s_NextAvailableBinding++;
	}

	static void ReleaseBinding(uint32_t binding)
	{
		s_FreeBindings.insert(binding);
	}

	UniformBuffer::UniformBuffer()
	{
		
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
		ReleaseBinding(m_Binding);
	}

	void UniformBuffer::Create(uint32_t size)
	{
		if (m_RendererID)
		{
			glDeleteBuffers(1, &m_RendererID);
			ReleaseBinding(m_Binding);
		}

		m_Binding = AllocateBinding();
		m_Size    = size;

		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_RendererID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}
