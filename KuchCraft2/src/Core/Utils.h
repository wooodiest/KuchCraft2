///
/// @file Utils.h
/// 
/// @brief Header file providing utility extensions for common things.
/// 
/// @details This file contains template specializations for std::tuple_size and std::tuple_element,
///          allowing glm vector types to be used with standard C++ tuple-like interfaces. Additionally,
///          it provides utility functions to access elements of these vectors using std::get.
/// 
/// @note This file is intended to grow with additional utility functions and structures that may
///       be useful across the application.
/// 
#pragma once

namespace std {

	/// Specialization of tuple_size for glm::vec2.
	template <>
	struct tuple_size<glm::vec2> : std::integral_constant<std::size_t, 2> {};

	/// Specialization of tuple_size for glm::vec3.
	template <>
	struct tuple_size<glm::vec3> : std::integral_constant<std::size_t, 3> {};

	/// Specialization of tuple_size for glm::vec4.
	template <>
	struct tuple_size<glm::vec4> : std::integral_constant<std::size_t, 4> {};

	/// Specialization of tuple_element for glm::vec2, defining the type for each index.
	template <std::size_t N>
	struct tuple_element<N, glm::vec2> {
		using type = float;
	};

	/// Specialization of tuple_element for glm::vec3, defining the type for each index.
	template <std::size_t N>
	struct tuple_element<N, glm::vec3> {
		using type = float;
	};

	/// Specialization of tuple_element for glm::vec4, defining the type for each index.
	template <std::size_t N>
	struct tuple_element<N, glm::vec4> {
		using type = float;
	};

	/// Retrieves the N-th element of a glm::vec2 using std::get.
	/// N must be less than 2, otherwise a static assertion will trigger.
	/// @tparam N - the index of the element to retrieve.
	/// @param v - the glm::vec2 vector.
	/// @return The N-th element of the vector.
	template <std::size_t N>
	float get(const glm::vec2& v) {
		static_assert(N < 2, "Index out of bounds for glm::vec2");
		if constexpr (N == 0) return v.x;
		if constexpr (N == 1) return v.y;
	}

	/// Retrieves the N-th element of a glm::vec3 using std::get.
	/// N must be less than 3, otherwise a static assertion will trigger.
	/// @tparam N - the index of the element to retrieve.
	/// @param v - the glm::vec3 vector.
	/// @return The N-th element of the vector.
	template <std::size_t N>
	float get(const glm::vec3& v) {
		static_assert(N < 3, "Index out of bounds for glm::vec3");
		if constexpr (N == 0) return v.x;
		if constexpr (N == 1) return v.y;
		if constexpr (N == 2) return v.z;
	}

	/// Retrieves the N-th element of a glm::vec4 using std::get.
	/// N must be less than 4, otherwise a static assertion will trigger.
	/// @tparam N - the index of the element to retrieve.
	/// @param v - the glm::vec4 vector.
	/// @return The N-th element of the vector.
	template <std::size_t N>
	float get(const glm::vec4& v) {
		static_assert(N < 4, "Index out of bounds for glm::vec4");
		if constexpr (N == 0) return v.x;
		if constexpr (N == 1) return v.y;
		if constexpr (N == 2) return v.z;
		if constexpr (N == 3) return v.w;
	}

	/// Specialization of tuple_size for glm::ivec2.
	template <>
	struct tuple_size<glm::ivec2> : std::integral_constant<std::size_t, 2> {};

	/// Specialization of tuple_size for glm::ivec3.
	template <>
	struct tuple_size<glm::ivec3> : std::integral_constant<std::size_t, 3> {};

	/// Specialization of tuple_size for glm::ivec4.
	template <>
	struct tuple_size<glm::ivec4> : std::integral_constant<std::size_t, 4> {};

	/// Specialization of tuple_element for glm::ivec2, defining the type for each index.
	template <std::size_t N>
	struct tuple_element<N, glm::ivec2> {
		using type = int;
	};

	/// Specialization of tuple_element for glm::ivec3, defining the type for each index.
	template <std::size_t N>
	struct tuple_element<N, glm::ivec3> {
		using type = int;
	};

	/// Specialization of tuple_element for glm::ivec4, defining the type for each index.
	template <std::size_t N>
	struct tuple_element<N, glm::ivec4> {
		using type = int;
	};

	/// Retrieves the N-th element of a glm::ivec2 using std::get.
	/// N must be less than 2, otherwise a static assertion will trigger.
	/// @tparam N - the index of the element to retrieve.
	/// @param v - the glm::ivec2 vector.
	/// @return The N-th element of the vector.
	template <std::size_t N>
	int get(const glm::ivec2& v) {
		static_assert(N < 2, "Index out of bounds for glm::ivec2");
		if constexpr (N == 0) return v.x;
		if constexpr (N == 1) return v.y;
	}

	/// Retrieves the N-th element of a glm::ivec3 using std::get.
	/// N must be less than 3, otherwise a static assertion will trigger.
	/// @tparam N - the index of the element to retrieve.
	/// @param v - the glm::ivec3 vector.
	/// @return The N-th element of the vector.
	template <std::size_t N>
	int get(const glm::ivec3& v) {
		static_assert(N < 3, "Index out of bounds for glm::ivec3");
		if constexpr (N == 0) return v.x;
		if constexpr (N == 1) return v.y;
		if constexpr (N == 2) return v.z;
	}

	/// Retrieves the N-th element of a glm::ivec4 using std::get.
	/// N must be less than 4, otherwise a static assertion will trigger.
	/// @tparam N - the index of the element to retrieve.
	/// @param v - the glm::ivec4 vector.
	/// @return The N-th element of the vector.
	template <std::size_t N>
	int get(const glm::ivec4& v) {
		static_assert(N < 4, "Index out of bounds for glm::ivec4");
		if constexpr (N == 0) return v.x;
		if constexpr (N == 1) return v.y;
		if constexpr (N == 2) return v.z;
		if constexpr (N == 3) return v.w;
	}



}