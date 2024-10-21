///
/// @file BufferLayout.h
/// 
/// @author Michal Kuchnicki
/// 

#pragma once

namespace KuchCraft {

	/// Enum class representing different shader data types.
	enum class ShaderDataType
	{
		None = 0,
		Uint, Int,
		Float, Float2, Float3, Float4
	};

	/// Structure representing an individual buffer element in a layout.
	struct BufferElement
	{
		/// The data type of the buffer element.
		ShaderDataType Type;

		/// The name of the buffer element
		std::string Name;

		/// The size of the buffer element in bytes.
		uint32_t Size = 0;

		/// The offset of the buffer element in the buffer layout
		uint32_t Offset = 0;

		/// Constructs a BufferElement with the specified type and name.
		/// This constructor initializes the Name and Type, and computes the Size
		/// of the buffer element based on its type.
		/// @param type - the data type of the buffer element.
		/// @param name - the name of the buffer element.
		BufferElement(ShaderDataType type, const std::string& name);

		/// Gets the count of components in the buffer element based on its type.
		/// @return - the number of components (1 for Uint, Int, Float; 2 for Float2, etc...)
		inline [[nodiscard]] uint32_t GetCount() const;

		/// Gets the size of the buffer element in bytes based on its type.
		/// @return the size in bytes
		inline [[nodiscard]] uint32_t GetSize() const;

	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		/// Constructs a BufferLayout with the specified buffer elements.
		/// This constructor initializes the buffer layout with the provided elements
		/// and computes their offsets and the overall stride.
		/// @param bufferElements - an initializer list of BufferElement objects.
		BufferLayout(const std::initializer_list<BufferElement>& bufferElements);

		/// Gets the stride of the buffer layout.
		/// The stride is the total size in bytes of all elements in the layout.
		/// @return the stride of the buffer layout in bytes.
		inline [[nodiscard]] uint32_t GetStride() const { return m_Stride; }

		/// Iterators for range-based for loops.
		inline [[nodiscard]] std::vector<BufferElement>::iterator begin()             { return m_BufferElements.begin(); }
		inline [[nodiscard]] std::vector<BufferElement>::iterator end()               { return m_BufferElements.end();   }
		inline [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
		inline [[nodiscard]] std::vector<BufferElement>::const_iterator end()   const { return m_BufferElements.end();   }

	private:
		/// Vector of buffer elements in the layout.
		std::vector<BufferElement> m_BufferElements;

		/// Total size in bytes of the buffer layout.
		uint32_t m_Stride = 0;

	};

}

