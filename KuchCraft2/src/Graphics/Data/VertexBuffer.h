///
/// @file VertexBuffer.h
/// 
/// @author Michal Kuchnicki
/// 

#pragma once

#include "Graphics/Data/BufferLayout.h"

namespace KuchCraft {

	/// Enum class defining the usage of the vertex buffer data.
	enum class VertexBufferDataUsage
	{
		/// Data that does not change over time
		STATIC = 0,

		/// Data that may change frequently
		DYNAMIC = 1
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;

		/// Cleaning up the vertex buffer
		~VertexBuffer();

		/// Creates a vertex buffer with the specified usage, size, and optionally, initial data.
		/// @param usage - defines whether the buffer data is static or dynamic
		/// @param size - the size of the buffer in bytes.
		/// @param data - pointer to the initial data to store in the buffer. If null, the buffer will be created without data.
		void Create(VertexBufferDataUsage usage, uint32_t size, const void* data = nullptr);

		/// Updates the data in the vertex buffer.
		/// @param size - the size of the data in bytes.
		/// @param data - pointer to the data to store in the buffer.
		void SetData(uint32_t size, const void* data);

		/// Sets the buffer layout, which describes how the vertex data is organized.
		/// @param bufferLayout - the layout of the buffer data.
		void SetBufferLayout(const BufferLayout& bufferLayout) { m_BufferLayout = bufferLayout; }

		/// Gets the current buffer layout.
		/// @return A const reference to the buffer layout.
		inline [[nodiscard]] const BufferLayout& GetBufferLayout() const { return m_BufferLayout; }

		/// Binds the vertex buffer, making it the current active buffer.
		void Bind() const;

		/// Unbinds the vertex buffer, clearing the current active buffer.
		void Unbind() const;

	private:
		/// ID of the OpenGL buffer object
		uint32_t m_RendererID = 0;

		/// The layout of the buffer, describing how vertex data is structured.
		BufferLayout m_BufferLayout;

		/// Usage type of the buffer (static or dynamic).
		VertexBufferDataUsage m_Usage = VertexBufferDataUsage::DYNAMIC;
	};

}