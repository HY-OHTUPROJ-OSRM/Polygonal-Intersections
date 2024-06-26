#include "traffic.h"
#include "interval_map.h"
#include <boost/test/unit_test.hpp>

#include <iostream>

#include "../../tests/test_utils.h"

using Value = IntervalMap::Value;

std::ostream& operator<<(std::ostream& os, const Value& value);

struct TestSpeedZones
{
	TrafficZones zones {
		.roadblock_polygons = {},
		.roadblock_polychains = {},

		.speed_zones = {
			SpeedZone{SpeedZone::offset, -20},
			SpeedZone{SpeedZone::factor, 1.5},
			SpeedZone{SpeedZone::cap, 70},
			SpeedZone{SpeedZone::constant, 50}
		}
	};

	TestSpeedZones()
	{
		zones.speed_zones.at(0).vertices = {
			{2, 1},
			{15, 1},
			{15, 12},
			{2, 12}
		};

		zones.speed_zones.at(1).vertices = {
			{8, 18},
			{8, 6},
			{22, 6},
			{22, 18}
		};

		zones.speed_zones.at(2).vertices = {
			{10, 8},
			{10, -2},
			{20, -2},
			{20, 8}
		};

		zones.speed_zones.at(3).vertices = {
			{4, 10},
			{12, 10},
			{12, 20},
			{4, 20}
		};

	}
};

BOOST_FIXTURE_TEST_SUITE(NewAverageSpeedTest, TestSpeedZones)

BOOST_AUTO_TEST_CASE(fully_in_offset_zone)
{
	auto speed = LineSegment{{3, 2}, {7, 9}}.new_average_speed(50, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 30);
}

BOOST_AUTO_TEST_CASE(fully_in_factor_zone)
{
	auto speed = LineSegment{{21, 8}, {13, 15}}.new_average_speed(50, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 75);
}

BOOST_AUTO_TEST_CASE(fully_in_cap_zone_1)
{
	auto speed = LineSegment{{20, 6}, {12, -2}}.new_average_speed(50, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 50);
}

BOOST_AUTO_TEST_CASE(fully_in_cap_zone_2)
{
	auto speed = LineSegment{{20, 6}, {12, -2}}.new_average_speed(100, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 70);
}

BOOST_AUTO_TEST_CASE(fully_in_cap_zone_3)
{
	auto speed = LineSegment{{20, 6}, {12, -2}}.new_average_speed(70, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 70);
}

BOOST_AUTO_TEST_CASE(fully_in_constant_zone)
{
	auto speed = LineSegment{{8, 19}, {5, 13}}.new_average_speed(70, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 50);
}

BOOST_AUTO_TEST_CASE(through_offset_zone_into_factor_zone)
{
	auto speed = LineSegment{{-3, 9}, {20, 9}}.new_average_speed(100, zones.speed_zones);

	BOOST_CHECK_CLOSE(speed, 93.559322033902, 1e-10);
}

BOOST_AUTO_TEST_CASE(through_all_zones)
{
	auto speed = LineSegment{{2, 22}, {22, 2}}.new_average_speed(100, zones.speed_zones);

	BOOST_CHECK_CLOSE(speed, 66.376926116161, 1e-10);
}

BOOST_AUTO_TEST_CASE(segment_length_zero)
{
	auto speed = LineSegment{{6, 18}, {6, 18}}.new_average_speed(100, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 100);
}

BOOST_AUTO_TEST_CASE(speed_zero)
{
	zones.speed_zones.at(3).speed_value = 0;
	auto speed = LineSegment{{6, 18}, {6, 14}}.new_average_speed(0, zones.speed_zones);

	BOOST_CHECK_EQUAL(speed, 0);
}

BOOST_AUTO_TEST_SUITE_END()
