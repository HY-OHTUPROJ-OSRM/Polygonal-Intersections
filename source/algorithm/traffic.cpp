#include "interval_map.h"
#include "traffic.h"
#include <algorithm>
#include <functional>

double SpeedZone::transform_speed(double speed) const
{
	switch (type)
	{
	case offset:
		return speed + speed_value;
	case factor:
		return speed * speed_value;
	case cap:
		return std::min(speed, speed_value);
	case constant:
	default:
		return speed_value;
	}
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

	bool travered = false;
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
			travered = true;
			traversed_dist += dist;
			traversed_time += dist / speed;
		}
		else if (dist > 0)
			return 0;
	}

	if (!travered) return og_speed;

	const double remaining_dist = total_dist - traversed_dist;

	if (remaining_dist > 0)
		traversed_time += remaining_dist / og_speed;

	return total_dist / traversed_time;
}
