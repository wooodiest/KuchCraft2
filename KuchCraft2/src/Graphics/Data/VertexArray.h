///
/// @file VertexArray.h
/// 
/// @author Michal Kuchnicki
/// 
/// 
#pragma once

#include "Graphics/Data/VertexBuffer.h"

namespace KuchCraft {

	class VertexArray
	{
	public:
		VertexArray() = default;

		/// Cleaning up the vertex array
		~VertexArray();

		/// Creates the vertex array and generates an ID for it.
		void Create();

        /// Sets the vertex buffer for this vertex array.
        /// Binds the vertex buffer to the vertex array and sets up the layout.
        /// @param vertexBuffer - the vertex buffer to be attached to this vertex array.
		void SetVertexBuffer(const VertexBuffer& vertexBuffer);

		/// Binds the vertex array, making it the current one to use in OpenGL.
		void Bind() const;

		/// Unbinds the vertex array, making no vertex array bound in OpenGL.
		void Unbind() const;

	private:
		/// ID of the OpenGL vertex array
		uint32_t m_RendererID = 0;

	};

}