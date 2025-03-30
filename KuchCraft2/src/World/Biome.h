#pragma once

#include "World/Item.h"

#include <vector>
#include <string>

namespace KuchCraft {

	struct BiomeInfo
	{
		std::string Name;

		struct TerrainInfo
		{
			int MaxHeight;
			int MinHeight;
			float Roughness;
			ItemID SurfaceBlock;
			ItemID SubSurfaceBlock;

		} Terrain;

		struct Vegetation
		{
			float VegetationFactor;
			/// ItemID, SpawnChance
			std::vector<std::pair<ItemID, float>> Data;

		} Vegetation;
	};

}