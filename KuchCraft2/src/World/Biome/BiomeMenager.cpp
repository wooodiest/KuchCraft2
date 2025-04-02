#include "kcpch.h"
#include "BiomeMenager.h"

#include "Core/Config.h"
#include "World/Item/ItemMenager.h"

#include <json.hpp>

namespace KuchCraft {

	void BiomeMenager::Reload()
	{
		std::ifstream file(ApplicationConfig::GetWorldData().BiomePackFile);
		if (!file.is_open())
		{
			Log::Error("[BiomeMenager] : Failed to open : {}", ApplicationConfig::GetWorldData().BiomePackFile);
			return;
		}
		nlohmann::json json;
		file >> json;
		file.close();
	
		if (!json.contains("biomes"))
		{
			Log::Error("[BiomeMenager] : No 'Biomes' key in biome pack file");
			return;
		}
	
		for (const auto& biome : json["biomes"])
		{
			if (!biome.contains("name"))
			{
				Log::Error("[BiomeMenager] : Biome needs name");
				continue;
			}

			if (!biome.contains("id"))
			{
				Log::Error("[BiomeMenager] : Biome needs id");
				continue;
			}
	
			std::string name = biome["name"];
			s_Data[name] = BiomeInfo{};
			BiomeInfo& info = s_Data[name];
			info.Name = name;
			info.ID = biome["id"];

			if (biome.contains("terrain"))
			{
				const auto& terrain = biome["terrain"];
				try
				{
					info.Terrain.Roughness          = terrain["roughness"];
					info.Terrain.MinContinentalness = terrain["minContinentalness"];
					info.Terrain.MaxContinentalness = terrain["maxContinentalness"];

					if (terrain["surfaceBlock"].is_number_integer())
						info.Terrain.SurfaceBlock = terrain["surfaceBlock"];
					else
						info.Terrain.SurfaceBlock = ItemMenager::GetItemIDByName(terrain["surfaceBlock"]);

					if (terrain["subSurfaceBlock"].is_number_integer())
						info.Terrain.SubSurfaceBlock = terrain["subSurfaceBlock"];
					else
						info.Terrain.SubSurfaceBlock = ItemMenager::GetItemIDByName(terrain["subSurfaceBlock"]);
				}
				catch (const std::exception& e)
				{
					Log::Error("[BiomeMenager] : Biome '{}' has invalid 'terrain' data", name);
					return;
				}
			}
			else
			{
				Log::Error("[BiomeMenager] : Biome '{}' missing 'terrain' data", name);
				return;
			}

			if (biome.contains("climate"))
			{
				const auto& climate = biome["climate"];
				try
				{
					info.Climate.MinTemperature = climate["minTemperature"];
					info.Climate.MaxTemperature = climate["maxTemperature"];
					info.Climate.MinHumidity    = climate["minHumidity"];
					info.Climate.MaxHumidity    = climate["maxHumidity"];

				}
				catch (const std::exception& e)
				{
					Log::Error("[BiomeMenager] : Biome '{}' has invalid 'climate' data", name);
					return;
				}
			}
	
		}
	}

}
