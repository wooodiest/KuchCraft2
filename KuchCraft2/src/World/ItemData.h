#pragma once

#include <stdint.h>
#include <cctype>
#include <string>
#include <algorithm>

#include "Graphics/Data/Primitives.h"

namespace KuchCraft
{
	/// Number of faces per block
	inline constexpr uint32_t block_face_count   = 6;

	/// Total number of vertices per block
	inline constexpr uint32_t block_vertex_count = 24;

	/// Total number of indices per block
	inline constexpr uint32_t block_index_count  = 36;

	/// Unique identifier for items
	using ItemID = uint16_t;

	/// Enum representing different item types
	enum class ItemType : uint8_t
	{
		Block,
		Item,
		Tool,
		Armor,
		Weapon,
		Food,
		Utility,
		Flower,
		Sapling,
		Other
	};

	/// Enum representing specific item data values
	enum class ItemData : ItemID
	{
		Air        = 0,
		DebugBlock = 1,
		Stone      = 2,
		Dirt       = 3,
		GrassBlock = 4,
		Gravel     = 5
	};

	/// Enum representing possible rotations for an item.
	enum class ItemRotation : uint8_t
	{
		DEG_0   = 0,
		DEG_90  = 1,
		DEG_180 = 2,
		DEG_270 = 3
	};

	/// Converts an ItemType enum to a string representation
	inline std::string ItemTypeToString(ItemType type)
	{
		switch (type)
		{
			case ItemType::Block:    return "block";
			case ItemType::Item:     return "item";
			case ItemType::Tool:     return "tool";
			case ItemType::Weapon:   return "weapon";
			case ItemType::Armor:    return "armor";
			case ItemType::Food:     return "food";
			case ItemType::Utility:  return "utility";
			case ItemType::Flower:   return "flower";
			case ItemType::Sapling:  return "sapling";
			case ItemType::Other:    return "other";
		}
	}

	/// Converts a string representation to an ItemType enum
	inline ItemType StringToItemType(const std::string& data)
	{
		std::string text = data;
		std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
			return std::tolower(c);
		});
		if (text == "block")
			return ItemType::Block;
		else if (text == "item")
			return ItemType::Item;
		else if (text == "tool")
			return ItemType::Tool;
		else if (text == "armor")
			return ItemType::Armor;
		else if (text == "weapon")
			return ItemType::Weapon;
		else if (text == "food")
			return ItemType::Food;
		else if (text == "flower")
			return ItemType::Flower;
		else if (text == "sapling")
			return ItemType::Sapling;
		else if (text == "utility")
			return ItemType::Utility;

		return ItemType::Other;
	}

	/// Enum representing different block faces
	enum class BlockFaces : uint8_t
	{
		Front  = 0,
		Left   = 1,
		Back   = 2,
		Right  = 3,
		Top    = 4,
		Bottom = 5
	};

	/// Texture mapping size per face
	inline constexpr float uvWidth  = 1.0f / block_face_count;
	inline constexpr float uvHeight = 1.0f;

	/// UV coordinates for each face of the block
	inline glm::vec2 blockFaceUV[block_face_count][quad_vertex_count] = {
		{ { 0.0f,        0.0f }, { uvWidth,     0.0f }, { uvWidth,     uvHeight }, { 0.0f,        uvHeight } }, // Front
		{ { uvWidth,     0.0f }, { 2 * uvWidth, 0.0f }, { 2 * uvWidth, uvHeight }, { uvWidth,     uvHeight } }, // Left
		{ { 2 * uvWidth, 0.0f }, { 3 * uvWidth, 0.0f }, { 3 * uvWidth, uvHeight }, { 2 * uvWidth, uvHeight } }, // Back
		{ { 3 * uvWidth, 0.0f }, { 4 * uvWidth, 0.0f }, { 4 * uvWidth, uvHeight }, { 3 * uvWidth, uvHeight } }, // Right
		{ { 4 * uvWidth, 0.0f }, { 5 * uvWidth, 0.0f }, { 5 * uvWidth, uvHeight }, { 4 * uvWidth, uvHeight } }, // Top
		{ { 5 * uvWidth, 0.0f }, { 6 * uvWidth, 0.0f }, { 6 * uvWidth, uvHeight }, { 5 * uvWidth, uvHeight } }  // Bottom
	};

	/// Position coordinates for each face of the block
	inline glm::vec3 blockFacePositions[block_face_count][quad_vertex_count] = {
		{ {-0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f} }, // Front
		{ {-0.5f, -0.5f, -0.5f }, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f} }, // Left
		{ { 0.5f, -0.5f, -0.5f }, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f} }, // Back
		{ { 0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f} }, // Right
		{ {-0.5f,  0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f} }, // Top
		{ {-0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f} }  // Bottom
	};

	/// Normal vectors for each face of the block
	inline glm::vec3 blockFaceNormals[block_face_count] = {
		{  0.0f,  0.0f,  1.0f }, // Front
		{ -1.0f,  0.0f,  0.0f }, // Left
		{  0.0f,  0.0f, -1.0f }, // Back
		{  1.0f,  0.0f,  0.0f }, // Right
		{  0.0f,  1.0f,  0.0f }, // Top
		{  0.0f, -1.0f,  0.0f }  // Bottom
	};
}