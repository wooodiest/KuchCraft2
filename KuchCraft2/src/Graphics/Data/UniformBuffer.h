#pragma once

namespace KuchCraft {

	class UniformBuffer
	{
	public:
		/// Initializes an empty UniformBuffer without allocating any GPU memory.
		UniformBuffer();

		/// Deletes GPU resources associated with this UniformBuffer and releases its binding number
		~UniformBuffer();

		/// Creates a UBO on the GPU and assigns it a binding.
		/// @param size - the size of the buffer in bytes to be allocated on the GPU.
		void Create(uint32_t size);

		/// Updates data in the UBO.
		/// @param data - a pointer to the data to be uploaded to the buffer.
		/// @param size - the size of the data in bytes to be uploaded.
		/// @param offset - the offset in bytes from the start of the buffer where the data should be written.
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		/// Get size of uniform buffer
		uint32_t GetSize() const { return m_Size; }

		/// Get binding of uniform buffer
		uint32_t GetBinding() const { return m_Binding; }

	private:
		/// OpenGL ID for the buffer object
		uint32_t m_RendererID = 0;

		/// The size of the buffer in bytes
		uint32_t m_Size = 0;

		/// The binding assigned to this buffer
		uint32_t m_Binding = 0;
	};

}