#include "line_segment.h"
#include <cassert>
#include <utility>

std::optional<Rational> find_intersection(const LineSegment& ls1, const LineSegment& ls2)
{
	const auto dir1 = ls1.b - ls1.a;
	const auto dir2 = ls2.b - ls2.a;

	const auto det = dir1.x * dir2.y - dir1.y * dir2.x;
	const auto offset = ls2.a - ls1.a;

	if (det == 0) // If the line segments are parallel
	{
		if (dir1.x * offset.y != dir1.y * offset.x || dir2.x * offset.y != dir2.y * offset.x)
			return std::nullopt; // The line segments are parallel but not collinear

		if (ls2.contains_projection(ls1.a))
			return {{0}}; // The first segment starts on the second segment

		const auto& [first_point, min_dot] = [&]() -> std::pair<const Vector2&, int64_t>
		{
			const int64_t dot_a = Dot(dir1, ls2.a - ls1.a);
			const int64_t dot_b = Dot(dir1, ls2.b - ls1.a);

			if (dot_a < dot_b)
				return {ls2.a, dot_a};
			else
				return {ls2.b, dot_b};
		}();

		const int64_t magSquared = Dot(dir1, dir1);

		if (0 < min_dot && min_dot <= magSquared)
			return {{min_dot, magSquared}};
		else
			return std::nullopt;
	}

	const Rational t1 = {dir2.y * offset.x - dir2.x * offset.y, det};
	const Rational t2 = {dir1.y * offset.x - dir1.x * offset.y, det};

	if (t1.nom * det < 0 ||
		t2.nom * det < 0 ||
		t1.nom * det > det * det ||
		t2.nom * det > det * det
	)
		return std::nullopt;

	assert(ls1.eval(t1) == ls2.eval(t2));

	return t1;
}
