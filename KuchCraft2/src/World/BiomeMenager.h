#pragma once

#include "World/Biome.h"

namespace KuchCraft {

	class BiomeMenager
	{
	public:
		/// Reloads item data from the configuration file.
		static void Reload();

		/// Retrieves biome information of all biomes.
		/// @return A reference to the map of BiomeInfo.
		static const std::unordered_map<std::string, BiomeInfo>& Get() { return s_Data; }

	private:
		/// Storage for biome information.
		static inline std::unordered_map<std::string, BiomeInfo> s_Data;
	};
}