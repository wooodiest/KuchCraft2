#include "kcpch.h"
#include "Graphics/Data/Texture.h"

namespace KuchCraft {

	void Texture::Bind(uint32_t rendererID, uint32_t slot)
	{
		glBindTextureUnit(slot, rendererID);
	}

	GLenum Texture::ImageFormatToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R8:     return GL_RED;
			case ImageFormat::RGB8:   return GL_RGB;
			case ImageFormat::RGBA8:  return GL_RGBA;
			case ImageFormat::RGBA32F:return GL_RGBA;
			case ImageFormat::None:   break;         
		}

		Log::Error("[Texture] : Unsupported image format");
		return GL_FALSE;
	}

	GLenum Texture::ImageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R8:      return GL_R8;      
			case ImageFormat::RGB8:    return GL_RGB8;    
			case ImageFormat::RGBA8:   return GL_RGBA8;   
			case ImageFormat::RGBA32F: return GL_RGBA32F; 
			case ImageFormat::None:    break;             
		}

		Log::Error("[Texture] : Unsupported image format");
		return GL_FALSE;
	}

	GLenum Texture::ImageFilterToGL(ImageFilter filter)
	{
		switch (filter)
		{
			case ImageFilter::LINEAR:  return GL_LINEAR;
			case ImageFilter::NEAREST: return GL_NEAREST;
		}

		Log::Error("[Texture] : Unsupported image filter");
		return GL_FALSE;
	}

	void Texture::CopyTo(std::shared_ptr<Texture> destination, int srcLayer, int dstLayer) const
	{
		if (!destination || GetRendererID() == 0 || destination->GetRendererID() == 0)
			Log::Error("[Texture] Error: Invalid texture IDs for copy operation.\n");

		glCopyImageSubData(
			GetRendererID(), GetOpenGLTextureType(), 0, 0, 0, srcLayer,
			destination->GetRendererID(), destination->GetOpenGLTextureType(), 0, 0, 0, dstLayer,
			GetWidth(), GetHeight(), 1
		);
	}

	uint32_t Texture::ImageFormatToChannelCount(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R8:     return 1;
			case ImageFormat::RGB8:   return 3;
			case ImageFormat::RGBA8:  return 4;
			case ImageFormat::RGBA32F:return 4;
			case ImageFormat::None:   break;
		}

		Log::Error("[Texture] : Unsupported image format");
		return GL_FALSE;
	}

	GLenum Texture::GetOpenGLTextureType() const
	{
		switch (GetSpecification().Type)
		{
			case TextureType::_2D:       return GL_TEXTURE_2D;
			case TextureType::_2D_ARRAY: return GL_TEXTURE_2D_ARRAY;
		}
		Log::Error("[Texture] : Unsupported texture type");
		return 0;
	}
}
