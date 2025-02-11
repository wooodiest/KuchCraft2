#pragma once

#include <stdint.h>
#include <cctype>
#include <string>
#include <algorithm>

#include "Graphics/Data/Primitives.h"

namespace KuchCraft
{
	inline constexpr uint32_t block_face_count = 6;

	using ItemID = uint8_t;

	enum class ItemType : uint8_t
	{
		Block,
		Tool,
		Food,
		Utility
	};

	enum class ItemData : ItemID
	{
		Air        = 0,
		Stone      = 1,
		Dirt       = 2,
		GrassBlock = 3,
		Apple      = 4
	};

	inline std::string ItemTypeToString(ItemType type)
	{
		switch (type)
		{
			case ItemType::Block:    return "block";
			case ItemType::Tool:     return "tool";
			case ItemType::Food:     return "food";
			case ItemType::Utility:  return "utility";
		}
	}

	inline ItemType StringToItemType(const std::string& data)
	{
		std::string text = data;
		std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
			return std::tolower(c);
		});
		if (text == "block")
			return ItemType::Block;
		else if (text == "tool")
			return ItemType::Tool;
		else if (text == "food")
			return ItemType::Food;
		else if (text == "utility")
			return ItemType::Utility;

		return ItemType::Utility;
	}

	enum class BlockFaces
	{
		Front  = 0,
		Left   = 1,
		Back   = 2,
		Right  = 3,
		Top    = 4,
		Bottom = 5
	};

	inline constexpr float uvWidth = 1.0f / block_face_count;
	inline constexpr float uvHeight = 1.0f;

	inline glm::vec2 blockFaceUV[block_face_count][quad_vertex_count] = {
		{ { 0.0f,        0.0f }, { uvWidth,     0.0f }, { uvWidth,     uvHeight }, { 0.0f,        uvHeight } }, // Front
		{ { uvWidth,     0.0f }, { 2 * uvWidth, 0.0f }, { 2 * uvWidth, uvHeight }, { uvWidth,     uvHeight } }, // Left
		{ { 2 * uvWidth, 0.0f }, { 3 * uvWidth, 0.0f }, { 3 * uvWidth, uvHeight }, { 2 * uvWidth, uvHeight } }, // Back
		{ { 3 * uvWidth, 0.0f }, { 4 * uvWidth, 0.0f }, { 4 * uvWidth, uvHeight }, { 3 * uvWidth, uvHeight } }, // Right
		{ { 4 * uvWidth, 0.0f }, { 5 * uvWidth, 0.0f }, { 5 * uvWidth, uvHeight }, { 4 * uvWidth, uvHeight } }, // Top
		{ { 5 * uvWidth, 0.0f }, { 6 * uvWidth, 0.0f }, { 6 * uvWidth, uvHeight }, { 5 * uvWidth, uvHeight } }  // Bottom
	};

	inline glm::vec3 blockFacePositions[block_face_count][quad_vertex_count] = {
		{ {-0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f} }, // Front
		{ {-0.5f, -0.5f, -0.5f }, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f} }, // Left
		{ { 0.5f, -0.5f, -0.5f }, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f} }, // Back
		{ { 0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f} }, // Right
		{ {-0.5f,  0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f} }, // Top
		{ {-0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f} }  // Bottom
	};

	inline glm::vec3 blockFaceNormals[block_face_count] = {
		{  0.0f,  0.0f,  1.0f }, // Front
		{ -1.0f,  0.0f,  0.0f }, // Left
		{  0.0f,  0.0f, -1.0f }, // Back
		{  1.0f,  0.0f,  0.0f }, // Right
		{  0.0f,  1.0f,  0.0f }, // Top
		{  0.0f, -1.0f,  0.0f }  // Bottom
	};
}