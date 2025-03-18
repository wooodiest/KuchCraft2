#include "kcpch.h"
#include "ViewFrustum.h"

namespace KuchCraft {
	
	ViewFrustum::ViewFrustum(const glm::mat4& viewProjection)
	{
		ExtractPlanes(viewProjection);
	}

	void ViewFrustum::ExtractPlanes(const glm::mat4& viewProjection)
	{
		m_FrustumPlanes[0] = glm::row(viewProjection, 3) + glm::row(viewProjection, 0);
		m_FrustumPlanes[1] = glm::row(viewProjection, 3) - glm::row(viewProjection, 0);
		m_FrustumPlanes[2] = glm::row(viewProjection, 3) + glm::row(viewProjection, 1);
		m_FrustumPlanes[3] = glm::row(viewProjection, 3) - glm::row(viewProjection, 1);
		m_FrustumPlanes[4] = glm::row(viewProjection, 3) + glm::row(viewProjection, 2);
		m_FrustumPlanes[5] = glm::row(viewProjection, 3) - glm::row(viewProjection, 2);

		for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++) 
		{
			float length = glm::length(glm::vec3(m_FrustumPlanes[i]));
			m_FrustumPlanes[i] /= length;
		}
	}

	bool ViewFrustum::ContainsAABBCompletely(const AABB& aabb) const
	{
		for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
		{
			glm::vec3 normal = glm::vec3(m_FrustumPlanes[i]);
			float d = m_FrustumPlanes[i].w;

			for (int j = 0; j < 8; j++)
			{
				glm::vec3 corner = glm::vec3(
					(j & 1) ? aabb.Max.x : aabb.Min.x,
					(j & 2) ? aabb.Max.y : aabb.Min.y,
					(j & 4) ? aabb.Max.z : aabb.Min.z);

				if (glm::dot(normal, corner) + d < 0.0f)
					return false;
			}
		}
		return true;
	}

	bool ViewFrustum::IsPointVisible(const glm::vec3& point) const
	{
		for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
		{
			float distance = glm::dot(glm::vec3(m_FrustumPlanes[i]), point) + m_FrustumPlanes[i].w;
			if (distance < 0.0f)
				return false;
		}

		return true;
	}

	bool ViewFrustum::IsAABBVisible(const AABB& aabb) const
	{
		for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++) 
		{
			glm::vec3 normal = glm::vec3(m_FrustumPlanes[i]);
			float d = m_FrustumPlanes[i].w;

			float dist = glm::dot(normal, glm::vec3(
				(normal.x < 0.0f) ? aabb.Min.x : aabb.Max.x,
				(normal.y < 0.0f) ? aabb.Min.y : aabb.Max.y,
				(normal.z < 0.0f) ? aabb.Min.z : aabb.Max.z)) + d;

			if (dist < 0.0f)
				return false;
		}

		return true;
	}
}
