#include "traffic.h"
#include "interval_map.h"
#include <boost/test/unit_test.hpp>
#include <optional>
#include <map>

template<auto nodes>
struct MockPathNodeReader
{
	uint64_t num_nodes = nodes.size();

	std::optional<PathNode> operator()()
	{
		if (num_nodes == 0) return std::nullopt;

		return nodes.at(nodes.size() - num_nodes--);
	}
};

struct NodeIDs
{
	uint64_t start, end;

	auto operator<=>(const NodeIDs&) const = default;
};

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const NodeIDs& node_ids)
{
	return os << "NodeIDs{" << node_ids.start << ", " << node_ids.end << '}';
}

struct MockSegmentWriter
{
	std::multimap<NodeIDs, double> written_segments;

	void operator()(const PathNode& start, const PathNode& end, double new_speed)
	{
		written_segments.emplace(NodeIDs{start.id, end.id}, new_speed);
	}

	void check(auto&&... correct_values) const
	{
		BOOST_CHECK_EQUAL(written_segments.size(), sizeof...(correct_values));

		if (written_segments.size() != sizeof...(correct_values)) return;

		std::array<decltype(written_segments)::value_type, sizeof...(correct_values)> arr = {correct_values...};

		for (std::size_t i = 0; auto& [node_ids, speed] : written_segments)
		{
			const auto& [correct_ids, correct_speed] = arr[i++];

			BOOST_CHECK_EQUAL(node_ids, correct_ids);
			BOOST_CHECK_CLOSE(speed, correct_speed, 1e-10);
		}
	}
};

using Segment = decltype(MockSegmentWriter().written_segments)::value_type;

struct TestTrafficZones
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

	MockSegmentWriter segment_writer;

	TestTrafficZones()
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

	static constexpr auto road = std::to_array<PathNode>({
		{{-3, 9}, 0},
		{{20, 9}, 1},
		{{7, -4}, 2},
		{{-4, 0}, 3}
	});
};

BOOST_FIXTURE_TEST_SUITE(NewAverageSpeedTest, TestTrafficZones)

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

BOOST_FIXTURE_TEST_SUITE(ForEachAffectedSegmentTest, TestTrafficZones)

BOOST_AUTO_TEST_CASE(for_each_affected_segment_no_roadblocks)
{
	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 93.559322033902},
		Segment{{1, 2}, 78.674351585011}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_1)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{4, 4},
		{0, 10},
		{7, 11}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 0},
		Segment{{1, 2}, 78.674351585011}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_2)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{10, -6},
		{4, -6},
		{6, 0}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 93.559322033902},
		Segment{{1, 2}, 0},
		Segment{{2, 3}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_3)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{18, 8},
		{20, 10},
		{21, 9}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 0},
		Segment{{1, 2}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_4)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{-6, 8},
		{26, 6},
		{24, 12},
		{-4, 14}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 0},
		Segment{{1, 2}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_5)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{-8, -4},
		{10, -6},
		{12, 0},
		{-6, 2}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 93.559322033902},
		Segment{{1, 2}, 0},
		Segment{{2, 3}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_6)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{4, -4},
		{10, -6},
		{24, 10},
		{18, 12}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 0},
		Segment{{1, 2}, 0},
		Segment{{2, 3}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_polygonal_roadblock_7)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{
		{-8, 0},
		{10, -6},
		{24, 10},
		{0, 16}
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 0},
		Segment{{1, 2}, 0},
		Segment{{2, 3}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_empty_polygonal_roadblock)
{
	zones.roadblock_polygons.components.emplace_back(std::initializer_list<Vector2_32>{});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 93.559322033902},
		Segment{{1, 2}, 78.674351585011}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_chain_roadblock_1)
{
	zones.roadblock_polychains.components.emplace_back(std::initializer_list<Vector2_32>{
		{-6, 2},
		{8, 0},
		{10, -10},
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 93.559322033902},
		Segment{{1, 2}, 0}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_chain_roadblock_2)
{
	zones.roadblock_polychains.components.emplace_back(std::initializer_list<Vector2_32>{
		{6, 14},
		{8, 0},
		{-6, 2},
	});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 0},
		Segment{{1, 2}, 78.674351585011}
	);
}

BOOST_AUTO_TEST_CASE(for_each_affected_segment_with_empty_chain_roadblock)
{
	zones.roadblock_polychains.components.emplace_back(std::initializer_list<Vector2_32>{});

	zones.for_each_affected_segment(100, MockPathNodeReader<road>(), segment_writer);

	segment_writer.check(
		Segment{{0, 1}, 93.559322033902},
		Segment{{1, 2}, 78.674351585011}
	);
}

BOOST_AUTO_TEST_SUITE_END()
