#pragma once

#include "Graphics/Data/Texture.h"

namespace KuchCraft {

	class TextureArray : public Texture 
	{
	public:
		/// Creates an empty TextureArray based on the given specification.
		TextureArray(const TextureSpecification& specification, const std::filesystem::path& path);

		/// Destroys the texture and releases resources.
		virtual ~TextureArray();

		/// Returns the texture's specification.
		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		/// Returns the texture width.
		virtual uint32_t GetWidth() const override { return m_Specification.Width; }

		/// Returns the texture height.
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }

		/// Returns the OpenGL renderer ID.
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		/// Sets raw data for a specific texture layer.
		void SetLayerData(uint32_t layer, void* data);

		/// Binds the texture to a specified texture slot.
		virtual void Bind(uint32_t slot = 0) const override;

		/// Checks if the texture was successfully loaded.
		virtual bool IsLoaded() const override { return m_IsLoaded; }

		/// Retrieves the file path associated with the texture.
		/// @return A reference to the file path used to load texture.
		virtual const std::filesystem::path& GetPath() const override { return m_Path; }

		/// Not used
		virtual void SetData(void* data, uint32_t size) override {}

	private:
		/// Texture specification settings.
		TextureSpecification m_Specification;

		/// OpenGL renderer ID.
		uint32_t m_RendererID = 0;

		/// The file path of the texture resource info file (if loaded from a file).
		std::filesystem::path m_Path;

		/// Indicates whether the texture was successfully loaded.
		bool m_IsLoaded = false;

	};

}
