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

		Log::Error("Unsupported image format");
		return GL_FALSE;
	}

	GLenum Texture::ImageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R8:     return GL_R8;      
			case ImageFormat::RGB8:   return GL_RGB8;    
			case ImageFormat::RGBA8:  return GL_RGBA8;   
			case ImageFormat::RGBA32F:return GL_RGBA32F; 
			case ImageFormat::None:   break;             
		}

		Log::Error("Unsupported image format");
		return GL_FALSE;
	}

	GLenum Texture::ImageFilterToGL(ImageFilter filter)
	{
		switch (filter)
		{
			case ImageFilter::LINEAR:  return GL_LINEAR;
			case ImageFilter::NEAREST: return GL_NEAREST;
		}

		Log::Error("Unsupported image filter");
		return GL_FALSE;
	}
}
