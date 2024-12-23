#pragma once

#include "Graphics/Data/Texture.h"

namespace KuchCraft {

	/// Manages loading, reloading, and accessing textures.
	class TextureManager
	{
	public:
		/// Loads a texture from the given file path.
		/// If the texture is already loaded, it returns the cached texture.
		/// @param path - the file path of the texture to load.
		/// @param specification - the texture specifications.
		/// @return A shared pointer to the loaded texture, or nullptr if loading fails.
		static std::shared_ptr<Texture> Load(const std::filesystem::path& path, const TextureSpecification& specification = TextureSpecification());

		/// Reloads a specific texture from the file path.
		/// @param path - the file path of the texture to reload.
		static void Reload(const std::filesystem::path& path);

		/// Reloads all cached textures.
		/// Iterates through all textures in the cache and reloads them.
		static void ReloadAll();

		/// Retrieves a texture from the cache.
		/// @param path - the file path of the texture to retrieve.
		/// @return A reference to the shared pointer of the texture.
		static std::shared_ptr<Texture>& Get(const std::filesystem::path& path) { return s_Data[path]; }

		/// Provides access to all cached textures.
		/// @return A constant reference to the texture cache.
		static const auto& GetData() { return s_Data; }

	private:
		/// Stores the mapping between file paths and their corresponding loaded textures.
		inline static std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> s_Data;

	private:
		TextureManager() = delete;
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

	};

}