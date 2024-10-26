///
/// @file UUID.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file defining the UUID Universally Unique Identifier system.
///
/// @details This file contains the definition of the UUID class, used to create 
///          unique identifiers within the application. This UUID system is designed 
///          to ensure reliable uniqueness across different components of the application, 
///          making it ideal for identifying entities or resources in a consistent and 
///          performant way. Additionally, a specialized hash function is provided for 
///          use in hash-based containers like std::unordered_map.
/// 
#pragma once

namespace KuchCraft {

	class UUID
	{
	public:
		/// Default constructor, generates a unique 64-bit identifier
		UUID();
		/// Constructs a UUID from an existing 64-bit value.
		/// @param uuid - the 64-bit value to be used as the UUID.
		UUID(uint64_t uuid);

		/// Copy constructor (default).
		UUID(const UUID&) = default;

		/// Conversion operator for UUID to uint64_t.
		/// Allows UUID instances to be implicitly converted to a uint64_t 
		/// value, which can be useful when UUID needs to be stored or passed 
		/// as a raw numeric identifier. 
		inline operator uint64_t() const { return m_UUID; }

	private:
		/// The internal 64-bit identifier value.
		uint64_t m_UUID;

	};

}

namespace std {

	/// Specialization of std::hash for UUIDs.
	/// Provides a hashing function for UUID instances to enable their usage
	/// in standard hash-based containers like std::unordered_map. The hash 
	/// function simply casts the UUID to a uint64_t and uses that as the hash.
	template <typename T> struct hash;
	template<>
	struct hash<KuchCraft::UUID>
	{
		std::size_t operator()(const KuchCraft::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}