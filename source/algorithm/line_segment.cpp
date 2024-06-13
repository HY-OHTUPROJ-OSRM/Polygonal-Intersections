#include "line_segment.h"
#include "interval_map.h"
#include "speed_zone.h"
#include <algorithm>
#include <functional>
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

struct SpeedZoneHandle
{
	std::reference_wrapper<const Polygon> polygon;

	double speed;
};

double LineSegment::new_average_speed(double og_speed, const std::vector<SpeedZone>& og_speed_zones) const
{
	std::vector<SpeedZoneHandle> speed_zones;
	speed_zones.reserve(og_speed_zones.size());

	for (auto& zone : og_speed_zones)
		speed_zones.emplace_back(zone, zone.transform_speed(og_speed));

	// Sort the speed zones from fastest to slowest
	std::sort(speed_zones.begin(), speed_zones.end(), [](const auto& zone1, const auto& zone2)
	{
		return zone1.speed > zone2.speed;
	});

	IntervalMap intervals;

	for (std::size_t i = 0; i < speed_zones.size(); ++i)
		intervals.insert_at_intersections(*this, speed_zones[i].polygon, i + 1);

	const double total_dist = length();

	double traversed_dist = 0;
	double traversed_time = 0;

	for (const auto& interval : intervals)
	{
		if (interval.mapped == 0) continue;

		const auto speed = speed_zones[interval.mapped - 1].speed;
		const auto portion = static_cast<double>(interval.end) - static_cast<double>(interval.start);
		const auto dist = total_dist * portion;

		if (speed > 0)
		{
			traversed_dist += dist;
			traversed_time += dist / speed;
		}
		else if (dist > 0)
			return 0;
	}

	const double remaining_dist = total_dist - traversed_dist;

	if (remaining_dist > 0)
		traversed_time += remaining_dist / og_speed;

	return total_dist / traversed_time;
}
