#pragma once

#include "AABB.h"

namespace KuchCraft {

	/// Number of planes in the view frustum
	constexpr uint32_t FRUSTUM_PLANES_COUNT = 6;

	/// Represents a view frustum used for frustum culling.
	class ViewFrustum
	{
	public:
		/// Default constructor.
		ViewFrustum() = default;

		/// Constructs a view frustum from a given view-projection matrix.
		/// @param viewProjection The view-projection matrix to extract planes from.
		ViewFrustum(const glm::mat4& viewProjection);

		/// Extracts the six planes from a given view-projection matrix.
		/// @param viewProjection The view-projection matrix.
		void ExtractPlanes(const glm::mat4& viewProjection);

		/// Checks if an axis-aligned bounding box (AABB) is within the frustum.
		/// @param aabb The AABB to check.
		/// @return True if the AABB is visible, false otherwise.
		bool IsAABBVisible(const AABB& aabb) const;

		/// Checks if an AABB is completely inside the frustum.
		/// @param aabb The AABB to check.
		/// @return True if the AABB is fully contained, false otherwise.
		bool ContainsAABBCompletely(const AABB& aabb) const;

		/// Gets the six frustum planes.
		/// @return Pointer to the array of frustum planes.
		const glm::vec4* GetPlanes() const { return m_FrustumPlanes; }

		/// Checks if a point is inside the frustum.
		/// @param point The point to check.
		/// @return True if the point is inside the frustum, false otherwise.
		bool IsPointVisible(const glm::vec3& point) const;

	private:
		/// The six frustum planes stored as vec4 (normal.xyz, distance.w).
		glm::vec4 m_FrustumPlanes[FRUSTUM_PLANES_COUNT];
	};
}