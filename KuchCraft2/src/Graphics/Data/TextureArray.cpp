#include "kcpch.h"
#include "TextureArray.h"

namespace KuchCraft {

	TextureArray::TextureArray(const TextureSpecification& specification, const std::filesystem::path& path)
		: m_Specification(specification), m_Path(path)
	{
		GLenum internalFormat = Texture::ImageFormatToGLInternalFormat(m_Specification.Format);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, m_Specification.Width, m_Specification.Height, m_Specification.Layers);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, Texture::ImageFilterToGL(m_Specification.Filter));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, Texture::ImageFilterToGL(m_Specification.Filter));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_IsLoaded = true;
	}

	TextureArray::~TextureArray()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void TextureArray::SetLayerData(uint32_t layer, void* data)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, m_Specification.Width, m_Specification.Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void TextureArray::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}
