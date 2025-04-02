#pragma once

#include "World/Item/Item.h"
#include "Graphics/Data/TextureArray.h"

namespace KuchCraft {

	class ItemMenager
	{
	public:
		/// Reloads item data from the configuration file.
		static void Reload();

		/// Retrieves item information based on its ID.
		/// @param id The ID of the item.
		/// @return A reference to the ItemInfo structure.
		static const ItemInfo& GetInfo(ItemID id) { return s_Data[id]; }

		/// Retrieves all item informations
		/// @return A reference to map of ItemInfo.
		static inline std::unordered_map<ItemID, ItemInfo>& GetData() { return s_Data; }

		/// Retrieves the ItemID based on its name.
		/// @param name The name of the item.
		/// @return The corresponding ItemID, or 0 if not found.
		static ItemID GetItemIDByName(const std::string& name)
		{
			if (s_NameData.find(name) != s_NameData.end())
				return s_NameData[name];
			else
				return 0;
		}

		static const uint32_t GetTextureLayer(ItemID id) { return s_ItemTextureArrayLayers[id]; }

		static const std::shared_ptr<TextureArray>& GetTextureArray() { return s_ItemTextureArray; }

	private:
		/// Path to the item configuration file.
		static inline std::filesystem::path s_Path;

		/// Storage for item information.
		static inline std::unordered_map<ItemID, ItemInfo> s_Data;

		/// Storage for item information.
		static inline std::unordered_map<std::string, ItemID> s_NameData;

		/// Stores all item textures
		static inline std::shared_ptr<TextureArray> s_ItemTextureArray;

		/// Maps item IDs to texture layers.
		static inline std::unordered_map<ItemID, uint32_t>  s_ItemTextureArrayLayers;

	};

}