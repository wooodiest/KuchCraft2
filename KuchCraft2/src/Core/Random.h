/// 
/// @file Random.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Random class, which provides static methods
///        for generating random numbers.
/// 
/// @details The Random class utilizes the C++ standard library's random number generation facilities.
///          It provides various methods for generating random integers and floating-point numbers.
///          This class is designed for use in multi-threaded applications, with each thread maintaining
///          its own instance of the random number generator. The Init() method should be called once
///          per thread to ensure that the random engine is seeded properly.
/// 
/// @note The Init() method must be called before any random number generation methods. Failure to do so
///       will result in undefined behavior.
/// 
/// @warning If using the Random class across multiple threads, ensure that the Init() method is called
///          on each thread to initialize the random engine properly. Failing to do so can lead to 
///          inconsistent random number generation across threads.
/// 
/// @example
///         // Initializing the random engine at the start of the thread.
///         Random::Init();
///         
///         // Generating a random unsigned integer.
///         uint32_t randomUInt = Random::UInt();
/// 
///         // Generating a random integer within a specified range.
///         int randomIntInRange = Random::Int(-10, 10);
/// 
///         // Generating a random floating-point number in the range [0.0, 1.0).
///         float randomFloat = Random::Float();
/// 

#pragma once

#include <random>

namespace KuchCraft {

	class Random
	{
	public:
		/// Initializes the random number generator for the current thread.
		/// This method seeds the random engine with a random value obtained from a random device.
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

	public:
		/// Generates a random unsigned integer.
		/// @return A random unsigned integer generated using the current thread's random engine.
		static uint32_t UInt()
		{
			return s_Distribution(s_RandomEngine);
		}

		/// Generates a random unsigned integer within a specified range.
		/// @param min - The minimum value of the range (inclusive).
		/// @param max - The maximum value of the range (inclusive).
		/// @return A random unsigned integer in the range [min, max] generated using the current thread's random engine.
		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		/// Generates a random integer.
		/// @return A random integer generated using the current thread's random engine.
		static int Int()
		{
			return static_cast<int>(UInt());
		}

		/// Generates a random integer within a specified range.
		/// @param min - The minimum value of the range (inclusive).
		/// @param max - The maximum value of the range (inclusive).
		/// @return A random integer in the range [min, max] generated using the current thread's random engine.
		static int Int(int min, int max)
		{
			return static_cast<int>(UInt(static_cast<uint32_t>(min), static_cast<uint32_t>(max)));
		}

		/// Generates a random floating-point number in the range [0.0, 1.0).
		/// @return A random float generated using the current thread's random engine.
		static float Float()
		{
			return static_cast<float>(s_Distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
		}

		/// Generates a random floating-point number within a specified range.
		/// @param min - The minimum value of the range (inclusive).
		/// @param max - The maximum value of the range (inclusive).
		/// @return A random float in the range [min, max] generated using the current thread's random engine.
		static float Float(float min, float max)
		{
			return min + (max - min) * Float();
		}

		/// Generates a random RGB color.
		/// @return A random glm::vec3 color with values in the range [0.0, 1.0] generated using the current thread's random engine.
		static glm::vec3 ColorRGB()
		{
			return glm::vec3(Float(0.0f, 1.0f), Float(0.0f, 1.0f), Float(0.0f, 1.0f));
		}

		/// Generates a random color in RGBA format.
		/// @return A random color represented as a glm::vec4 where alpha is set to 1.0f, generated using the current thread's random engine.
		static glm::vec4 ColorRGBA()
		{
			return glm::vec4(Float(0.0f, 1.0f), Float(0.0f, 1.0f), Float(0.0f, 1.0f), 1.0f);
		}		

		/// Generates a random position within the specified bounds.
		/// @param min - The minimum bounds for the position.
		/// @param max - The maximum bounds for the position.
		/// @return A random glm::vec3 position within the bounds generated using the current thread's random engine.
		static glm::vec3 Position(const glm::vec3& min, const glm::vec3& max)
		{
			return glm::vec3(Float(min.x, max.x), Float(min.y, max.y), Float(min.z, max.z));
		}

		/// Generates a random angle in radians.
		/// @return A random angle in the range [0, 2PI) generated using the current thread's random engine.
		static float Angle()
		{
			return Float(0.0f, glm::two_pi<float>());
		}

		/// Generates a random normalized direction vector.
		/// @return A random normalized glm::vec3 direction generated using the current thread's random engine.
		static glm::vec3 Direction()
		{
			float theta = Angle();
			float phi = glm::acos(2.0f * Float(0.0f, 1.0f) - 1.0f);
			return glm::vec3(glm::sin(phi) * glm::cos(theta), glm::sin(phi) * glm::sin(theta), glm::cos(phi));
		}

		/// Generates a random quaternion.
		/// @return A random glm::quat generated using the current thread's random engine.
		static glm::quat Quaternion()
		{
			float u1 = Float(0.0f, 1.0f);
			float u2 = Float(0.0f, 1.0f);
			float u3 = Float(0.0f, 1.0f);
			float sqrt1u1 = sqrt(1 - u1);
			return glm::quat(sqrt1u1 * sin(2 * glm::pi<float>() * u2),
				sqrt1u1 * cos(2 * glm::pi<float>() * u2),
				sqrt(u1) * sin(2 * glm::pi<float>() * u3),
				sqrt(u1) * cos(2 * glm::pi<float>() * u3));
		}

		/// Generates a random normalized vector.
		/// @return A random glm::vec3 normalized vector generated using the current thread's random engine.
		static glm::vec3 NormalizedVector()
		{
			return glm::normalize(Vec3());
		}

		/// Generates a random boolean value.
		/// @return A random boolean value, true or false generated using the current thread's random engine.
		static bool Bool()
		{
			return UInt(0, 1) == 1;
		}

		/// Generates a random glm::vec2.
		/// @return A random vector with two components in the range [0.0, 1.0) generated using the current thread's random engine.
		static glm::vec2 Vec2()
		{
			return glm::vec2(Float(), Float());
		}

		/// Generates a random glm::vec3.
		/// @return A random vector with three components in the range [0.0, 1.0) generated using the current thread's random engine.
		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		/// Generates a random glm::vec4.
		/// @return A random vector with four components in the range [0.0, 1.0) generated using the current thread's random engine.
		static glm::vec4 Vec4()
		{
			return glm::vec4(Float(), Float(), Float(), Float());
		}

		/// Generates a random glm::vec2 within a specified range.
		/// @param min - The minimum value for the vector components.
		/// @param max - The maximum value for the vector components.
		/// @return A random vector in the range [min, max] generated using the current thread's random engine.
		static glm::vec2 Vec2(float min, float max)
		{
			return glm::vec2(Float(min, max), Float(min, max));
		}

		/// Generates a random glm::vec3 within a specified range.
		/// @param min - The minimum value for the vector components.
		/// @param max - The maximum value for the vector components.
		/// @return A random vector in the range [min, max] generated using the current thread's random engine.
		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
		}

		/// Generates a random glm::vec4 within a specified range.
		/// @param min - The minimum value for the vector components.
		/// @param max - The maximum value for the vector components.
		/// @return A random vector in the range [min, max] generated using the current thread's random engine.
		static glm::vec4 Vec4(float min, float max)
		{
			return glm::vec4(Float(min, max), Float(min, max), Float(min, max), Float(min, max));
		}

		/// Generates a random glm::ivec2.
		/// @return A random integer vector with two components generated using the current thread's random engine.
		static glm::ivec2 IVec2()
		{
			return glm::ivec2(Int(), Int());
		}

		/// Generates a random glm::ivec3.
		/// @return A random integer vector with three components generated using the current thread's random engine.
		static glm::ivec3 IVec3()
		{
			return glm::ivec3(Int(), Int(), Int());
		}

		/// Generates a random glm::ivec4.
		/// @return A random integer vector with four components generated using the current thread's random engine.
		static glm::ivec4 IVec4()
		{
			return glm::ivec4(Int(), Int(), Int(), Int());
		}

		/// Generates a random glm::ivec2 within a specified range.
		/// @param min - The minimum value for the vector components.
		/// @param max - The maximum value for the vector components.
		/// @return A random integer vector in the range [min, max] generated using the current thread's random engine.
		static glm::ivec2 IVec2(int min, int max)
		{
			return glm::ivec2(Int(min, max), Int(min, max));
		}

		/// Generates a random glm::ivec3 within a specified range.
		/// @param min - The minimum value for the vector components.
		/// @param max - The maximum value for the vector components.
		/// @return A random integer vector in the range [min, max] generated using the current thread's random engine.
		static glm::ivec3 IVec3(int min, int max)
		{
			return glm::ivec3(Int(min, max), Int(min, max), Int(min, max));
		}

		/// Generates a random glm::ivec4 within a specified range.
		/// @param min - The minimum value for the vector components.
		/// @param max - The maximum value for the vector components.
		/// @return A random integer vector in the range [min, max] generated using the current thread's random engine.
		static glm::ivec4 IVec4(int min, int max)
		{
			return glm::ivec4(Int(min, max), Int(min, max), Int(min, max), Int(min, max));
		}

	private:
		/// Thread-local random number generator for each thread
		static thread_local inline std::mt19937 s_RandomEngine;

		/// Thread-local distribution used for generating random numbers
		static thread_local inline std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

	private:
		/// The Random class is a static utility class and cannot be instantiated.
		/// Constructors, destructors, and assignment operators are deleted to
		/// prevent instantiation and copying.
		Random() = delete;
		~Random() = delete;
		Random(const Random&) = delete;
		Random& operator=(const Random&) = delete;
		Random(Random&&) = delete;
		Random& operator=(Random&&) = delete;
	};
}