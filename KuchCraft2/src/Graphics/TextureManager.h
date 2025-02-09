#pragma once

#include "Graphics/Data/Texture.h"

namespace KuchCraft {

	/// Manages loading, reloading, and accessing textures.
	class TextureManager
	{
	public:
		/// Initializes the texture manager system.
		static void Init();

		/// Shuts down the texture manager system.
		static void Shutdown();

		/// Loads a texture from the given file path.
		/// If the texture is already loaded, it returns the cached texture.
		/// @param path - the file path of the texture to load.
		/// @param specification - the texture specifications.
		/// @return A shared pointer to the loaded texture, or nullptr if loading fails.
		static std::shared_ptr<Texture> Load(const std::filesystem::path& path, const TextureSpecification& specification = TextureSpecification());

		/// Adds a texture to the texture manager cache.
		/// @param tex - the shared pointer to the texture.
		/// @param name - the name or identifier for the texture.
		static void Add(const std::shared_ptr<Texture>& tex, const std::string& name);

		/// Reloads a specific texture from the file path.
		/// @param name - the file path/name of the texture to reload.
		static void Reload(const std::string& name);

		/// Reloads all cached textures.
		/// Iterates through all textures in the cache and reloads them.
		static void ReloadAll();

		/// Retrieves a texture from the cache.
		/// @param name - the file path/name of the texture to retrieve.
		/// @return A constant reference to the shared pointer of the texture.
		static const std::shared_ptr<Texture>& Get(const std::string& name) { return s_Data[name]; }

		/// Provides access to all cached textures.
		/// @return A constant reference to the texture cache.
		static const auto& GetData() { return s_Data; }

		/// Retrieves a white texture from.
		/// @return A constant reference to the texture.
		static const auto& GetWhiteTexture() { return s_WhiteTexture; }

	private:
		/// Stores the mapping between file paths/names and their corresponding loaded textures.
		inline static std::unordered_map<std::string, std::shared_ptr<Texture>> s_Data;

		/// 1x1 pixel white texture to be used when rendering just color without texture qued
		inline static std::shared_ptr<Texture> s_WhiteTexture;

	private:
		TextureManager() = delete;
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

	};

}