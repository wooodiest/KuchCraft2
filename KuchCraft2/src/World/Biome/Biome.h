#pragma once

#include "World/Item/Item.h"

#include <vector>
#include <string>

namespace KuchCraft {

	struct BiomeInfo
	{
		std::string Name;
		int ID;

		struct TerrainInfo
		{
			float MinContinentalness;
			float MaxContinentalness;

			float  Roughness;
			ItemID SurfaceBlock;
			ItemID SubSurfaceBlock;

		} Terrain;

		struct ClimateInfo
		{
			float MinTemperature;
			float MaxTemperature;
			float MinHumidity;
			float MaxHumidity;
		} Climate;
	};

}