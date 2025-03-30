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

    /// Represents the detailed information about an item or block.
    struct ItemInfo
    {
    ///// === BASIC PROPERTIES 
        /// Display name of the item/block.
        std::string Name = "AIR"; 

        /// A short description of the item.
        std::string Description;

        /// Defines if it's a block, tool, food, etc.
        ItemType Type = ItemType::Utility;

        /// Determines if the block is transparent.
        bool Transparent = true;

        /// Maximum number of items per stack.
        int StackSize = 64;

        /// Determines if the item can be crafted.
        bool IsCraftable = false; 

		/// TODO: Item recipes

    ///// === DURABILITY & BREAKING ===
        /// Determines how long the item lasts before breaking.
        int Durability = 1; 

        /// Time required to break this item/block.
        float BreakingTime = 1.0f;

        /// Tools that can break this item.
        std::unordered_set<ItemID> BreakableBy; 

        /// Items dropped when broken.
        std::vector<std::pair<ItemID, int>> Drops; 

    ///// === DEFENSE & DAMAGE ===
        /// Defense points for armor-type items.
        int Defense = 0; 

        /// Attack damage if the item is a weapon.
        int AttackDamage = 0; 

        /// Speed of attacking with this item.
        float AttackSpeed = 1.0f; 

    ///// === FOOD PROPERTIES ===
        /// Determines if the item can be eaten.
        bool IsEdible = false;

        /// How many hunger points this item restores.
        int FoodValue = 0; 
        
        /// Does eating this item give a potion effect?
        bool HasSpecialEffect = false; 

        /// Effects given when eaten (e.g., "Speed", 10s).
        std::vector<std::pair<std::string, float>> FoodEffects; 

    ///// === ENVIRONMENTAL PROPERTIES ===
        /// Determines how much light the item emits.
        float LightEmission = 0.0f; 

        /// Color of the emitted light.
		glm::vec4 LightColor = glm::vec4(1.0f); 

    ///// === PHYSICAL PROPERTIES ===
        /// Affects physics.
        float Weight = 1.0f; 

        /// Determines slipperiness.
        float Friction = 0.5f; 

    };

	class Item
	{
	public:
		Item();

		/// Constructor that initializes an item with a specific ID.
		/// @param id The unique identifier of the item.
		Item(ItemID id);

        /// Constructor that initializes an item with a specific name.
        /// @param name The name of the item.
        Item(const std::string& name);

		/// Constructor that initializes an item witch ItemData.
		/// @param data TItemData representing id of specific item.
		Item(ItemData data);

		/// Constructor that initializes an item with ItemData and rotation.
		/// @param data ItemData representing the ID of a specific item.
		/// @param rotation The rotation of the item.
		Item(ItemData data, ItemRotation rotation);

        /// Constructor that initializes an item with a specific ID and rotation.
        /// @param id The unique identifier of the item.
        /// @param rotation The rotation of the item.
        Item(ItemID id, ItemRotation rotation);

        /// Constructor that initializes an item with a specific name and rotation.
        /// @param name The name of the item
        /// @param rotation The rotation of the item.
        Item(const std::string& name, ItemRotation rotation);

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