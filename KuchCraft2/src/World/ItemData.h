#pragma once

#include <stdint.h>
#include <cctype>
#include <string>
#include <algorithm>

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
		if (data == "block")
			return ItemType::Block;
		else if (data == "tool")
			return ItemType::Tool;
		else if (data == "food")
			return ItemType::Food;
		else if (data == "utility")
			return ItemType::Utility;

		return ItemType::Utility;
	}

}