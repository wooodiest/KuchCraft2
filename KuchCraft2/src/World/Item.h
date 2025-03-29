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
		std::string Name        = "AIR";
		ItemType    Type        = ItemType::Utility;
		bool        Transparent = true;
	};

	class Item
	{
	public:
		Item();

		/// Constructor that initializes an item with a specific ID.
		/// @param id The unique identifier of the item.
		Item(ItemID id);

		/// Constructor that initializes an item witch ItemData.
		/// @param data TItemData representing id of specific item.
		Item(ItemData data);

		/// Constructor that initializes an item with ItemData and rotation.
		/// @param data ItemData representing the ID of a specific item.
		/// @param rotation The rotation of the item.
		Item(ItemData data, ItemRotation rotation);

		~Item();

		/// Handles item usage logic.
		void Use();

		/// Sets the rotation of the item.
		/// @param rotation The desired rotation value.
		inline void SetRotation(ItemRotation rotation) { m_Flags = (m_Flags & ~0x03) | ((uint8_t)rotation & 0x03); }

		/// Retrieves the current rotation of the item.
		/// @return The rotation value as an ItemRotation enum.
		inline ItemRotation GetRotation() const { return (ItemRotation)(m_Flags & 0x03); }

		/// Retrieves the unique identifier of the item.
	    /// @return The ID of the item.
		inline [[nodiscard]] ItemID GetID() const { return m_ID; }

		/// Retrieves the item information.
		/// @return A reference to the ItemInfo structure.
		[[nodiscard]] const ItemInfo& GetInfo() const;

		/// Retrieves the texture associated with a given item ID.
		/// @param id The ID of the item.
		/// @return A shared pointer to the texture.
		static [[nodiscard]] const std::shared_ptr<Texture>& GetTexture(ItemID id);

		/// Retrieves the texture associated with this specific item instance.
	    /// @return A shared pointer to the texture.
		[[nodiscard]] const std::shared_ptr<Texture>& GetTexture() const { return GetTexture(m_ID); };

	private:
		/// The unique identifier of the item.
		ItemID m_ID = (ItemID)ItemData::Air;
		
		/// Flags containing additional properties of the item.
		uint16_t m_Flags = 0;

	};

}