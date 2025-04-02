#pragma once

#include <glm/glm.hpp>

namespace KuchCraft {

	constexpr int max_spline_points = 20;

	struct Spline
	{
		glm::vec2 Points[max_spline_points] = { { 0, 0 }, { 1, 1 } };
		int Count = 2;

		float Apply(float t)
		{
			if (t <= 0)
				return Points[0].y;

			if (t >= 1)
				return Points[Count - 1].y;

			int i = 0;
			while (Points[i + 1].x < t)
				i++;

			float t0 = Points[i].x;
			float t1 = Points[i + 1].x;
			float p0 = Points[i].y;
			float p1 = Points[i + 1].y;

			return glm::mix(p0, p1, (t - t0) / (t1 - t0));
		}
	};

}