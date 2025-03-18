#include "kcpch.h"
#include "AABB.h"

namespace KuchCraft {

	bool AABB::IsColliding(const AABB& other) const
	{
		return (Min.x < other.Max.x && Max.x > other.Min.x &&
			Min.y < other.Max.y && Max.y > other.Min.y &&
			Min.z < other.Max.z && Max.z > other.Min.z);
	}

	AABB AABB::MoveTo(const glm::vec3& position) const
	{
		return AABB{
			Min + position,
			Max + position
		};
	}

	glm::vec3 AABB::GetOverlaping(const AABB& other) const
	{
		return glm::vec3{
			glm::min(Max.x, other.Max.x) - glm::max(Min.x, other.Min.x),
			glm::min(Max.y, other.Max.y) - glm::max(Min.y, other.Min.y),
			glm::min(Max.z, other.Max.z) - glm::max(Min.z, other.Min.z)
		};
	}

	void AABB::Expand(const glm::vec3& amount)
	{
		Min -= amount;
		Max += amount;
	}

	bool AABB::Contains(const glm::vec3& point) const
	{
		return (point.x >= Min.x && point.x <= Max.x &&
			point.y >= Min.y && point.y <= Max.y &&
			point.z >= Min.z && point.z <= Max.z);
	}

	AABB AABB::Merge(const AABB& other) const
	{
		return AABB{
			glm::min(Min, other.Min),
			glm::max(Max, other.Max)
		};
	}

	float AABB::Volume() const
	{
		glm::vec3 size = Max - Min;
		return size.x * size.y * size.z;
	}

	AABB AABB::Intersect(const AABB& other) const
	{
		glm::vec3 newMin = glm::max(Min, other.Min);
		glm::vec3 newMax = glm::min(Max, other.Max);
		if (newMin.x >= newMax.x || newMin.y >= newMax.y || newMin.z >= newMax.z)
			return AABB(); 
		return AABB(newMin, newMax);
	}
}
