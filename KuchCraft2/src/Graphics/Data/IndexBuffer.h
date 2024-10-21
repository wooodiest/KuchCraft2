///
/// @file IndexBuffer.h
/// 
/// @author Michal Kuchnicki
/// 

#pragma once

namespace KuchCraft {

	/// Enum class defining the usage of the index buffer data.
	enum class IndexBufferDataUsage
	{
		/// Data that does not change over time
		STATIC  = 0,

		/// Data that may change frequently
		DYNAMIC = 1
	};

	class IndexBuffer
	{
	public:
		IndexBuffer() = default;

		/// Cleaning up the index buffer
		~IndexBuffer();

		/// Creates and initializes the index buffer with the specified usage and data.
		/// This function creates a new index buffer with the provided usage type and data.
		/// If data is not provided, the buffer is initialized with uninitialized data.
		/// @param usage - the usage type of the buffer (STATIC or DYNAMIC).
		/// @param count - the number of indices to be stored in the buffer.
		/// @param data - optional pointer to the initial index data.
		void Create(IndexBufferDataUsage usage, uint32_t count, uint32_t* data = nullptr);

		/// Sets new data for the index buffer.
		/// This function updates the index data of the currently bound index buffer.
		/// @param data - pointer to the new index data.
		/// @param count - the number of indices in the new data.
		void SetData(uint32_t* data, uint32_t count);

		/// Binds the index buffer for rendering.
		void Bind() const;

		/// Unbinds the index buffer.
		void Unbind() const;

		/// Gets the number of indices in the buffer.
		/// @return the number of indices stored in the index buffer.
		inline [[nodiscard]] uint32_t GetCount() const { return m_Count; };

		/// Gets simple info about buffer
		/// @return the string reference containing info
		inline [[nodiscard]] const std::string& GetInfo() const { return m_Info; }

	private:
		/// ID of the OpenGL buffer object
		uint32_t m_RendererID = 0;

		/// Number of indices stored in the buffer.
		uint32_t m_Count = 0;

		/// Usage type of the buffer (static or dynamic).
		IndexBufferDataUsage m_Usage = IndexBufferDataUsage::STATIC;

		/// String representing buffer info
		std::string m_Info;

	};

}