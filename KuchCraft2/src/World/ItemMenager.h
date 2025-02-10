#pragma once

#include "World/Item.h"
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

	private:
		/// Path to the item configuration file.
		static inline std::filesystem::path s_Path;

		///<Storage for item information.
		static inline std::unordered_map<ItemID, ItemInfo> s_Data;

		/// Stores all item textures
		static inline std::shared_ptr<TextureArray> s_ItemTextureArray;

		/// Maps item IDs to texture layers.
		static inline std::unordered_map<ItemID, uint32_t>  s_ItemTextureArrayLayers;

	};

}