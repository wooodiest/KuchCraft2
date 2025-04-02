#include "kcpch.h"
#include "WorldGenerator.h"
#include "World/Chunk.h"
#include "World/World.h"
#include "World/BiomeMenager.h"

#include "Core/Config.h"

#include <imgui.h>
#include <glad/glad.h>
#include <json.hpp>
#include <FastNoiseSIMD.h>

namespace KuchCraft {

    void WorldGenerator::Reload(int seed)
    {
		Shutdown();

		s_Seed = seed;

		std::ifstream file(ApplicationConfig::GetWorldData().WorldGeneratorPackFile);
		if (!file.is_open())
		{
			Log::Error("[WorldGenerator] : Failed to open : {}", ApplicationConfig::GetWorldData().WorldGeneratorPackFile);
			return;
		}
		nlohmann::json json;
		file >> json;
		file.close();

		if (!json.contains("noises"))
		{
			Log::Error("[WorldGenerator] : No 'noises' key in world generator pack file");
			return;
		}

		for (const auto& noise : json["noises"])
		{
			if (!noise.contains("name"))
			{
				Log::Error("[WorldGenerator] : Noise needs name");
				continue;
			}

			NoiseData noiseData;
			std::string name = noise["name"];

			if (noise.contains("scale"))
				noiseData.Scale = noise["scale"];

			if (noise.contains("type"))
				noiseData.Type = noise["type"];

			if (noise.contains("cellularReturnType"))
				noiseData.CellularReturnType = noise["cellularReturnType"];

			if (noise.contains("frequency"))
				noiseData.Frequency = noise["frequency"];

			if (noise.contains("octaves"))
				noiseData.Octaves = noise["octaves"];

			if (noise.contains("perturbFractalOctaves"))
				noiseData.PerturbFractalOctaves = noise["perturbFractalOctaves"];

			if (noise.contains("power"))
				noiseData.Power = noise["power"];

			if (noise.contains("spline"))
			{
				const auto& spline = noise["spline"];
				for (int i = 0; i < spline.size(); i++)
				{
					noiseData.Spline.Points[i].x = spline[i][0];
					noiseData.Spline.Points[i].y = spline[i][1];
				}
				noiseData.Spline.Count = spline.size();
			}

			if (name == "ContinentalnessNoise")
				s_ContinentalnessNoise = noiseData;
			else if (name == "Continentalness2Noise")
				s_Continentalness2Noise = noiseData;
			else if (name == "ContinentalnessPick")
				s_ContinentalnessPick = noiseData;
			else if (name == "PeaksAndValiesNoise")
				s_PeaksAndValiesNoise = noiseData;
			else if (name == "PeaksAndValies2Noise")
				s_PeaksAndValies2Noise = noiseData;
			else if (name == "TemperatureNoise")
				s_TemperatureNoise = noiseData;
			else if (name == "HumidityNoise")
				s_HumidityNoise = noiseData;
			else if (name == "VegetationNoise")
				s_VegetationNoise = noiseData;
			else if (name == "ErosionNoise")
				s_ErosionNoise = noiseData;
			else
				Log::Error("[WorldGenerator] : Unknown noise name : {}", name);
		}

		auto setupNoise = [&](NoiseData& data, int seed) {
            data.Noise = FastNoiseSIMD::NewFastNoiseSIMD();
			data.Noise->SetSeed(seed);
			data.Noise->SetNoiseType(static_cast<FastNoiseSIMD::NoiseType>(data.Type));
			data.Noise->SetFrequency(data.Frequency);
			data.Noise->SetFractalOctaves(data.Octaves);
			data.Noise->SetCellularReturnType(static_cast<FastNoiseSIMD::CellularReturnType>(data.CellularReturnType));
			data.Noise->SetPerturbFractalOctaves(data.PerturbFractalOctaves);
		};

		setupNoise(s_ContinentalnessNoise,  s_Seed + 0  );
		setupNoise(s_Continentalness2Noise, s_Seed + 33 );
		setupNoise(s_ContinentalnessPick,   s_Seed + 43 );
		setupNoise(s_PeaksAndValiesNoise,   s_Seed + 53 );
		setupNoise(s_PeaksAndValies2Noise,  s_Seed + 63 );
		setupNoise(s_TemperatureNoise,      s_Seed + 73 );
		setupNoise(s_HumidityNoise,         s_Seed + 83 );	
		setupNoise(s_VegetationNoise,       s_Seed + 93 );
		setupNoise(s_ErosionNoise,          s_Seed + 103);
    }

    void WorldGenerator::Shutdown()
    {
		delete s_ContinentalnessNoise .Noise;
		delete s_Continentalness2Noise.Noise;
		delete s_ContinentalnessPick  .Noise;
		delete s_PeaksAndValiesNoise  .Noise;
		delete s_PeaksAndValies2Noise .Noise;
		delete s_TemperatureNoise     .Noise;
		delete s_HumidityNoise        .Noise;
		delete s_VegetationNoise      .Noise;
		delete s_ErosionNoise         .Noise;
    }

    void WorldGenerator::GenerateChunk(Chunk* chunk)
    {
        if (!chunk) 
            return;

        glm::vec3 position = chunk->GetPosition();

		auto apply = [&](std::array<float, chunk_size_XZ * chunk_size_XZ>& tab, NoiseData& data) {
			data.Noise->FillNoiseSet(tab.data(), position.x, position.y, position.z, chunk_size_XZ, 1, chunk_size_XZ);
			for (int i = 0; i < tab.size(); i++)
			{
				tab[i] += 1;
				tab[i] /= 2;
				tab[i] = glm::pow(tab[i], data.Power);
				tab[i] = data.Spline.Apply(tab[i]);
			}
		};

		std::array<float, chunk_size_XZ* chunk_size_XZ> continentalness, continentalness2, continentalnessPick;
		apply(continentalness,     s_ContinentalnessNoise);
		apply(continentalness2,    s_Continentalness2Noise);
		apply(continentalnessPick, s_ContinentalnessPick);

		for (int i = 0; i < chunk->m_Continentalness.size(); i++)
			chunk->m_Continentalness[i] = glm::mix(continentalness[i], continentalness2[i], continentalnessPick[i]);

		std::array<float, chunk_size_XZ* chunk_size_XZ> peaksAndValies, peaksAndValies2;
		apply(peaksAndValies,  s_PeaksAndValiesNoise);
		apply(peaksAndValies2, s_PeaksAndValies2Noise);

		for (int i = 0; i < chunk->m_PeaksAndValies.size(); i++)
			chunk->m_PeaksAndValies[i] = glm::mix(peaksAndValies[i], peaksAndValies2[i], continentalnessPick[i]);

		apply(chunk->m_Temperature, s_TemperatureNoise);
		apply(chunk->m_Humidity,    s_HumidityNoise);
		apply(chunk->m_Vegetation,  s_VegetationNoise);
		apply(chunk->m_Erosion,     s_ErosionNoise);

		const auto& biomes = BiomeMenager::Get();

		for (int x = 0; x < chunk_size_XZ; x++)
		{
			for (int z = 0; z < chunk_size_XZ; z++)
			{
				float temp = chunk->m_Temperature    [x * chunk_size_XZ + z];
				float hum  = chunk->m_Humidity       [x * chunk_size_XZ + z];
				float cont = chunk->m_Continentalness[x * chunk_size_XZ + z];

				BiomeInfo* selectedBiome = nullptr;
				float bestMatch = 1.0f;

				for (const auto& [name, curBiome] : biomes)
				{
					float tempDiff = glm::abs(temp - glm::mix(curBiome.Climate.MinTemperature,     curBiome.Climate.MaxTemperature,     0.5f));
					float humDiff  = glm::abs(hum  - glm::mix(curBiome.Climate.MinHumidity,        curBiome.Climate.MaxHumidity,        0.5f));
					float contDiff = glm::abs(cont - glm::mix(curBiome.Terrain.MinContinentalness, curBiome.Terrain.MaxContinentalness, 0.5f));
					float biomeMatch = tempDiff + humDiff + contDiff;

					if (biomeMatch < bestMatch)
					{
						bestMatch     = biomeMatch;
						selectedBiome = const_cast<BiomeInfo*>(&curBiome);
					}
				}

				if (!selectedBiome)
					selectedBiome = const_cast<BiomeInfo*>(&biomes.begin()->second);

				chunk->m_BiomesIDs[x * chunk_size_XZ + z] = selectedBiome->ID;

				int groundHeight = glm::mix(0.8f, 1.2f, chunk->m_PeaksAndValies[x * chunk_size_XZ + z]);

				for (int y = 0; y < chunk_size_Y; y++)
				{
					if (y > groundHeight)
						chunk->m_Data[x][y][z] = Item(ItemData::Air);
					else if (y == groundHeight)
						chunk->m_Data[x][y][z] = Item(selectedBiome->Terrain.SurfaceBlock);
					else if (y > groundHeight - 3)
						chunk->m_Data[x][y][z] = Item(selectedBiome->Terrain.SubSurfaceBlock);
					else if (y > groundHeight - 6 && chunk->m_Erosion[x * chunk_size_XZ + z] > 0.5f)
						chunk->m_Data[x][y][z] = Item(ItemData::Gravel);
					else
						chunk->m_Data[x][y][z] = Item(ItemData::Stone);
				}
			}
		}

    }
    void WorldGenerator::OnImGuiRender()
    {
        
    }

}
