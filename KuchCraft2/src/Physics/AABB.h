#pragma once

namespace KuchCraft {

	struct AABB
	{
		/// Minimum point of the bounding box
		glm::vec3 Min{ 0.0f };

		/// Maximum point of the bounding box
		glm::vec3 Max{ 0.0f };

		/// Default constructor initializes an empty AABB.
		AABB() = default;

		/// Constructs an AABB with specified min and max points.
		/// @param min The minimum point of the bounding box.
		/// @param max The maximum point of the bounding box.
		AABB(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}

		/// Gets the center position of the AABB.
		/// @return The center of the bounding box.
		glm::vec3 GetPosition() const { return (Max + Min) * 0.5f; }

		/// Gets the half-size of the AABB.
		/// @return Half the size of the bounding box.
		glm::vec3 GetSize() const { return (Max - Min) * 0.5f; }

		/// Checks if this AABB collides with another AABB.
		/// @param other The other AABB to check collision against.
		/// @return True if the AABBs are colliding, false otherwise.
		bool IsColliding(const AABB& other) const;

		/// Moves the AABB to a new position.
		/// @param position The new position to move the AABB to.
		/// @return A new AABB moved to the given position.
		AABB MoveTo(const glm::vec3& position) const;

		/// Computes the overlapping volume between two AABBs.
		/// @param other The other AABB to check overlap with.
		/// @return A vector representing the overlap size in each axis.
		glm::vec3 GetOverlaping(const AABB& other) const;

		/// Expands the AABB by a given vector.
		/// @param amount The amount to expand the AABB in all directions.
		void Expand(const glm::vec3& amount);

		/// Checks if a point is inside the AABB.
		/// @param point The point to check.
		/// @return True if the point is inside, false otherwise.
		bool Contains(const glm::vec3& point) const;

		/// Merges this AABB with another AABB, returning a new AABB that contains both.
		/// @param other The other AABB to merge with.
		/// @return A new AABB that encompasses both AABBs.
		AABB Merge(const AABB& other) const;

		/// Computes the volume of the AABB.
		/// @return The volume of the bounding box.
		float Volume() const;

		/// Computes the intersection of this AABB with another.
		/// @param other The other AABB.
		/// @return A new AABB representing the intersection. If no intersection, returns an empty AABB.
		AABB Intersect(const AABB& other) const;

	};

}