#include "kcpch.h"
#include "BiomeMenager.h"

#include "Core/Config.h"
#include "World/ItemMenager.h"

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
	
		if (!json.contains("Biomes"))
		{
			Log::Error("[BiomeMenager] : No 'Biomes' key in biome pack file");
			return;
		}
	
		for (const auto& biome : json["Biomes"])
		{
			if (!biome.contains("Name"))
			{
				Log::Error("[BiomeMenager] : Biome needs name");
				return;
			}
	
			std::string name = biome["Name"];
			s_Data[name] = BiomeInfo{};
			BiomeInfo& info = s_Data[name];
			info.Name = name;
	
			if (biome.contains("Terrain"))
			{
				const auto& terrain = biome["Terrain"];
				try
				{
					info.Terrain.MaxHeight       = terrain["MaxHeight"];
					info.Terrain.MinHeight       = terrain["MinHeight"];
					info.Terrain.Roughness       = terrain["Roughness"];

					if (terrain["SurfaceBlock"].is_number_integer())
						info.Terrain.SurfaceBlock = terrain["SurfaceBlock"];
					else
						info.Terrain.SurfaceBlock = ItemMenager::GetItemIDByName(terrain["SurfaceBlock"]);

					if (terrain["SubSurfaceBlock"].is_number_integer())
						info.Terrain.SubSurfaceBlock = terrain["SubSurfaceBlock"];
					else
						info.Terrain.SubSurfaceBlock = ItemMenager::GetItemIDByName(terrain["SubSurfaceBlock"]);
				}
				catch (const std::exception& e)
				{
					Log::Error("[BiomeMenager] : Biome '{}' has invalid 'Terrain' data", name);
					return;
				}
			}
			else
			{
				Log::Error("[BiomeMenager] : Biome '{}' missing 'Terrain' data", name);
				return;
			}
	
			if (biome.contains("Vegetation"))
			{
				const auto& vegetation = biome["Vegetation"];
				try
				{
					info.Vegetation.VegetationFactor = vegetation["VegetationFactor"];
					for (const auto& item : vegetation["Data"])
					{
						if (item["ID"].is_number_integer())
							info.Vegetation.Data.push_back({ item["ID"], item["Chance"] });
						else
							info.Vegetation.Data.push_back({ ItemMenager::GetItemIDByName(item["ID"]), item["Chance"] });
					}
				}
				catch (const std::exception& e)
				{
					Log::Error("[BiomeMenager] : Biome '{}' has invalid 'Vegetation' data", name);
					return;
				}
			}
			else
			{
				Log::Error("[BiomeMenager] : Biome '{}' missing 'Vegetation' data", name);
				return;
			}
		}
	}

}
