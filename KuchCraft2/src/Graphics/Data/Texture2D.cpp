#include "kcpch.h"
#include "Texture2D.h"

#include <stb_image.h>

namespace KuchCraft {

	Texture2D::Texture2D(const TextureSpecification& specification)
		: m_Specification(specification)
	{
		GLenum internalFormat = Texture::ImageFormatToGLInternalFormat(m_Specification.Format);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Specification.Width, m_Specification.Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Texture::ImageFilterToGL(m_Specification.Filter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Texture::ImageFilterToGL(m_Specification.Filter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		m_IsLoaded = true;
	}

	Texture2D::Texture2D(const TextureSpecification& specification, const std::filesystem::path& path)
		: m_Specification(specification), m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

		if (data)
		{
			m_Specification.Width  = width;
			m_Specification.Height = height;

			GLenum internalFormat = GL_FALSE;
			GLenum dataFormat     = GL_FALSE;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat     = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat     = GL_RGB;
			}
			else
			{
				Log::Error("Unsupported image format with {0} channels", channels);
				stbi_image_free(data);
				return;
			}

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Specification.Width, m_Specification.Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Texture::ImageFilterToGL(m_Specification.Filter));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Texture::ImageFilterToGL(m_Specification.Filter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, dataFormat, GL_UNSIGNED_BYTE, data);
			if (m_Specification.GenerateMips)
				glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
			m_IsLoaded = true;
		}
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::SetData(void* data, uint32_t size)
	{
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, Texture::ImageFormatToGLDataFormat(m_Specification.Format) , GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}
