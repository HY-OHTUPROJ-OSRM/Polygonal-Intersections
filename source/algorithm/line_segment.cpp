#include "line_segment.h"
#include <algorithm>
#include <tuple>

// Finds the intersection in terms of parameters to ls1
std::variant<std::monostate, Rational, RationalInterval>
find_intersection(const LineSegment& ls1, const LineSegment& ls2)
{
	const auto dir1 = ls1.b - ls1.a;
	const auto dir2 = ls2.b - ls2.a;

	const auto det = Det(dir1, dir2);
	const auto offset = ls2.a - ls1.a;

	if (det == 0) // If the line segments are parallel
	{
		if (dir1.x * offset.y != dir1.y * offset.x || dir2.x * offset.y != dir2.y * offset.x)
			return {}; // The line segments are parallel but not collinear

		const auto& [first_point, min_dot, max_dot] = [&]() -> std::tuple<const Vector2&, int64_t, int64_t>
		{
			const int64_t dot_a = Dot(dir1, ls2.a - ls1.a);
			const int64_t dot_b = Dot(dir1, ls2.b - ls1.a);

			if (dot_a < dot_b)
				return {ls2.a, dot_a, dot_b};
			else
				return {ls2.b, dot_b, dot_a};
		}();

		const int64_t magSquared = Dot(dir1, dir1);
		const int64_t end_dot = std::min(max_dot, magSquared);

		if (ls2.contains_projection(ls1.a)) // The first segment starts on the second segment
			return RationalInterval{0, end_dot, magSquared};

		if (0 < min_dot && min_dot <= magSquared)
			return RationalInterval{min_dot, end_dot, magSquared};

		return {};
	}

	const Rational t1 = {Det(offset, dir2), det};
	const Rational t2 = {Det(offset, dir1), det};

	if (t1.is_negative() ||
		t2.is_negative() ||
		t1.is_above_1() ||
		t2.is_above_1()
	)
		return {};

	return t1;
}
