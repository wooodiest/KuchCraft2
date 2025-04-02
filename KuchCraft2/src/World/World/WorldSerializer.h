#pragma once

#include "World/World/World.h"

namespace KuchCraft {

	/// A utility class for serializing and deserializing a World object.
	/// This class handles saving the state of a World to a file and restoring it later.
	class WorldSerializer
	{
	public:
		/// Constructs a WorldSerializer object with a given World instance.
	    /// @param world - pointer to the World instance to be serialized or deserialized.
		WorldSerializer(World* world);

		~WorldSerializer();

		/// Serializes the World to a file.
	    /// This method writes the current state of the World into a JSON file.
	    /// @return True if the serialization succeeds, false otherwise.
		bool Serialize();

		/// Serializes the runtime state of the World.
	    /// Not implemented yet, reserved for runtime-specific serialization.
	    /// @return False by default.
		bool SerializeRuntime();

		/// Deserializes the World from a file.
		/// This method reads the World state from a JSON file and reconstructs it.
		/// @return True if the deserialization succeeds, false otherwise.
		bool Deserialize();

		/// Deserializes the runtime state of the World.
	    /// Not implemented yet, reserved for runtime-specific deserialization.
	    /// @return False by default.
		bool DeserializeRuntime();

	private:
		/// Pointer to the World instance being serialized or deserialized.
		World* m_World;

	};

}