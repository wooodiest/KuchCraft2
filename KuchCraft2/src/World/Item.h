/// @file Item.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Item class and related structures.
///

#pragma once

#include "World/ItemData.h"

#include "Graphics/Data/Texture.h"

namespace KuchCraft {

	/// Represents the basic information about an item.
	struct ItemInfo
	{
		std::string Name = "NONE";
		ItemType    Type = ItemType::Utility;
	};

	class Item
	{
	public:
		Item() = default;

		/// Constructor that initializes an item with a specific ID.
		/// @param id The unique identifier of the item.
		Item(ItemID id)
			: m_ID(id) {}

		virtual ~Item() = default;

		///Virtual function for item usage
		virtual void Use() {}

		/// Retrieves the unique identifier of the item.
	    /// @return The ID of the item.
		inline [[nodiscard]] ItemID GetID() const { return m_ID; }

		/// Retrieves the texture associated with a given item ID.
		/// @param id The ID of the item.
		/// @return A shared pointer to the texture.
		static [[nodiscard]] const std::shared_ptr<Texture>& GetTexture(ItemID id);

		/// Retrieves the texture associated with this specific item instance.
	    /// @return A shared pointer to the texture.
		[[nodiscard]] const std::shared_ptr<Texture>& GetTexture() const { return GetTexture(m_ID); };

	protected:
		/// The unique identifier of the item.
		ItemID m_ID = (ItemID)ItemData::Air;

	};

}