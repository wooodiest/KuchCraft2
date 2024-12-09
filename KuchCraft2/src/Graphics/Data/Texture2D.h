#pragma once

#include "Graphics/Data/Texture.h"

namespace KuchCraft {

	class Texture2D : public Texture
	{
	public:
		/// Constructs a 2D texture with the specified texture configuration.
		Texture2D(const TextureSpecification& specification);

		/// Constructs a 2D texture by loading it from a file with the specified texture configuration
		Texture2D(const TextureSpecification& specification, const std::filesystem::path& path);

		/// Frees any allocated resources associated with the texture.
		virtual ~Texture2D();

		/// Retrieves the texture's specification.
		/// @return A reference to the `TextureSpecification` struct describing the texture.
		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		/// Retrieves the width of the texture.
		/// @return The width of the texture in pixels.
		virtual uint32_t GetWidth() const override { return m_Specification.Width; }

		/// Retrieves the height of the texture.
		/// @return The height of the texture in pixels.
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }

		/// Retrieves the OpenGl ID of the texture.
		/// @return The unique identifier used by the rendering API to reference this texture.
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		/// Retrieves the file path associated with the texture.
		/// @return A reference to the file path used to load texture.
		virtual const std::filesystem::path& GetPath() const override { return m_Path; }

		/// Sets the raw data for the texture.
		/// @param data - a pointer to the raw data to be uploaded to the texture.
		/// @param size - the size of the data in bytes.
		virtual void SetData(void* data, uint32_t size) override;

		/// Binds the texture to a specified texture slot for rendering.
		/// @param slot The texture slot to bind the texture to (default is slot 0).
		virtual void Bind(uint32_t slot = 0) const override;

		/// Checks whether the texture is successfully loaded.
		/// @return `true` if the texture is loaded successfully, otherwise `false`.
		virtual bool IsLoaded() const override { return m_IsLoaded; }

	private:
		/// The configuration and properties of the texture.
		TextureSpecification m_Specification;

		/// The file path of the texture resource (if loaded from a file).
		std::filesystem::path m_Path;

		/// The unique identifier assigned by OpenGl.
		uint32_t m_RendererID = 0;

		/// Indicates whether the texture was successfully loaded.
		bool m_IsLoaded = false;

	};

}