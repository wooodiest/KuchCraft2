#pragma once

#include <FastNoiseSIMD.h>

#include "Spline.h"

namespace KuchCraft {

    class Chunk;

    struct NoiseData
    {
		FastNoiseSIMD* Noise = nullptr;

		float Scale = 0.1f;
        int Type = FastNoiseSIMD::NoiseType::SimplexFractal;
        int CellularReturnType = FastNoiseSIMD::CellularReturnType::Distance;
        float Frequency = 0.015f;
        int Octaves = 3;
        int PerturbFractalOctaves = 0;
        float Power = 1.0f;
        Spline Spline;
    };

    class WorldGenerator
    {
    public:
        static void Reload(int seed);

		static void Shutdown();

        static void GenerateChunk(Chunk* chunk);

        static void OnImGuiRender();

    private:
        static inline int s_Seed = 1234;

        static inline NoiseData s_ContinentalnessNoise;
        static inline NoiseData s_Continentalness2Noise;
        static inline NoiseData s_ContinentalnessPick;

        static inline NoiseData s_PeaksAndValiesNoise;
		static inline NoiseData s_PeaksAndValies2Noise;

		static inline NoiseData s_TemperatureNoise;
		static inline NoiseData s_HumidityNoise;
		static inline NoiseData s_VegetationNoise;
		static inline NoiseData s_ErosionNoise;
    };

}
