#pragma once

#include <stdint.h>

namespace KuchCraft
{
	inline constexpr uint32_t block_face_count = 6;

	using ItemID = uint8_t;

	enum class ItemData : ItemID
	{
		Air        = 0,
		Stone      = 1,
		Dirt       = 2,
		GrassBlock = 3,
		Apple      = 4
	};
}