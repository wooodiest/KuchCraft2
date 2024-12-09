#pragma once

#include <glad/glad.h>

namespace KuchCraft {

	/// Represents various image formats for textures.
	/// These formats specify the color depth and layout of the texture data.
	enum class ImageFormat
	{
		/// No format specified
		None = 0,

		/// 8-bit single-channel (red) format
		R8,

		/// 8-bit three-channel (red, green, blue) format
		RGB8,

		/// 8-bit four-channel (red, green, blue, alpha) format
		RGBA8,

		/// 32-bit floating-point four-channel (red, green, blue, alpha) format
		RGBA32F

	};

	/// Specifies filtering options for textures.
	/// These filters determine how textures are sampled when rendered.
	enum class ImageFilter
	{
		/// Linear filtering for smoother texture sampling
		LINEAR = 0,

		/// Nearest-neighbor filtering for sharper texture sampling
		NEAREST

	};

	/// Describes the properties and configuration of a texture.
	struct TextureSpecification
	{
		/// The width of the texture in pixels
		uint32_t Width = 1;

		/// The height of the texture in pixels
		uint32_t Height = 1;

		/// The color format of the texture
		ImageFormat Format = ImageFormat::RGBA8;

		/// he filtering method for sampling the texture
		ImageFilter Filter = ImageFilter::LINEAR;

		/// Whether to generate mipmaps for the texture
		bool GenerateMips = true;

	};

	/// Abstract base class for managing textures. Provides a unified interface for texture-related operations.
	class Texture
	{
	public:
		virtual ~Texture() = default;

		/// Retrieves the texture's specification.
		/// @return A reference to the `TextureSpecification` struct describing the texture.
		virtual const TextureSpecification& GetSpecification() const = 0;

		/// Retrieves the width of the texture.
		/// @return The width of the texture in pixels.
		virtual uint32_t GetWidth() const = 0;

		/// Retrieves the height of the texture.
		/// @return The height of the texture in pixels.
		virtual uint32_t GetHeight() const = 0;

		/// Retrieves the OpenGl ID of the texture.
		/// @return The unique identifier used by the rendering API to reference this texture.
		virtual uint32_t GetRendererID() const = 0;

		/// Retrieves the file path associated with the texture.
		/// @return A reference to the file path used to load texture.
		virtual const std::filesystem::path& GetPath() const = 0;

		/// Sets the raw data for the texture.
		/// @param data - a pointer to the raw data to be uploaded to the texture.
		/// @param size - the size of the data in bytes.
		virtual void SetData(void* data, uint32_t size) = 0;

		/// Binds the texture to a specified texture slot for rendering.
		/// @param slot The texture slot to bind the texture to (default is slot 0).
		virtual void Bind(uint32_t slot = 0) const = 0;

		/// Checks whether the texture is successfully loaded.
		/// @return `true` if the texture is loaded successfully, otherwise `false`.
		virtual bool IsLoaded() const = 0;

		static void Bind(uint32_t rendererID, uint32_t slot);

		/// Helpers
		static GLenum ImageFormatToGLDataFormat(ImageFormat format);
		static GLenum ImageFormatToGLInternalFormat(ImageFormat format);
		static GLenum ImageFilterToGL(ImageFilter filter);
	};
}