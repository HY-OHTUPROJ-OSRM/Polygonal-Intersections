#include "interval_map.h"
#include "test_utils.h"
#include <boost/test/unit_test.hpp>

using Value = IntervalMap::Value;

std::ostream& operator<<(std::ostream& os, const Value& value)
{
	return os << "{ [" << value.start << ", " << value.end << ") -> " << value.mapped << " }";
}

void check_interval_map(const IntervalMap& map, const auto&&... correct_values)
{
	std::array<decltype(*std::begin(map)), sizeof...(correct_values)>
		arr = {correct_values...};

	BOOST_CHECK_EQUAL_COLLECTIONS(
		std::begin(map), std::end(map),
		std::begin(arr), std::end(arr)
	);
}

struct TestIntervalMap { IntervalMap map; };

BOOST_FIXTURE_TEST_SUITE(IntervalMapInsertionTest, TestIntervalMap)

BOOST_AUTO_TEST_CASE(empty)
{
	check_interval_map(map);
}

BOOST_AUTO_TEST_CASE(one_interval_1)
{
	map.insert({{1, 3}, {2, 3}}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(one_interval_2)
{
	map.insert({{0}, {1, 2}}, 11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(one_interval_3)
{
	map.insert({{1, 2}, {1}}, 11111);

	check_interval_map(map, Value{{1, 2}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(one_interval_4)
{
	map.insert({{0}, {1}}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(separate_intervals_same_value_1)
{
	map.insert({1, 5}, {2, 5}, 11111);
	map.insert({3, 5}, {4, 5}, 11111);

	check_interval_map(map,
		Value{{1, 5}, {2, 5}, 11111},
		Value{{2, 5}, {3, 5}, 0},
		Value{{3, 5}, {4, 5}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_same_value_2)
{
	map.insert({3, 5}, {4, 5}, 11111);
	map.insert({1, 5}, {2, 5}, 11111);

	check_interval_map(map,
		Value{{1, 5}, {2, 5}, 11111},
		Value{{2, 5}, {3, 5}, 0},
		Value{{3, 5}, {4, 5}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_same_value_3)
{
	map.insert({0}, {1, 3}, 11111);
	map.insert({2, 3}, {3, 4}, 11111);

	check_interval_map(map,
		Value{{0}, {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 0},
		Value{{2, 3}, {3, 4}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_same_value_4)
{
	map.insert({2, 3}, {3, 4}, 11111);
	map.insert({0}, {1, 3}, 11111);

	check_interval_map(map,
		Value{{0}, {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 0},
		Value{{2, 3}, {3, 4}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_same_value_5)
{
	map.insert({1, 2}, {2, 3}, 11111);
	map.insert({3, 4}, {1}, 11111);

	check_interval_map(map,
		Value{{1, 2}, {2, 3}, 11111},
		Value{{2, 3}, {3, 4}, 0},
		Value{{3, 4}, {1}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_same_value_6)
{
	map.insert({3, 4}, {1}, 11111);
	map.insert({1, 2}, {2, 3}, 11111);

	check_interval_map(map,
		Value{{1, 2}, {2, 3}, 11111},
		Value{{2, 3}, {3, 4}, 0},
		Value{{3, 4}, {1}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_different_values_1)
{
	map.insert({1, 5}, {2, 5}, 11111);
	map.insert({3, 5}, {4, 5}, 22222);

	check_interval_map(map,
		Value{{1, 5}, {2, 5}, 11111},
		Value{{2, 5}, {3, 5}, 0},
		Value{{3, 5}, {4, 5}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_different_values_2)
{
	map.insert({3, 5}, {4, 5}, 11111);
	map.insert({1, 5}, {2, 5}, 22222);

	check_interval_map(map,
		Value{{1, 5}, {2, 5}, 22222},
		Value{{2, 5}, {3, 5}, 0},
		Value{{3, 5}, {4, 5}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_different_values_3)
{
	map.insert({0}, {1, 3}, 11111);
	map.insert({2, 3}, {3, 4}, 22222);

	check_interval_map(map,
		Value{{0}, {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 0},
		Value{{2, 3}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_different_values_4)
{
	map.insert({2, 3}, {3, 4}, 11111);
	map.insert({0}, {1, 3}, 22222);

	check_interval_map(map,
		Value{{0}, {1, 3}, 22222},
		Value{{1, 3}, {2, 3}, 0},
		Value{{2, 3}, {3, 4}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_different_values_5)
{
	map.insert({1, 2}, {2, 3}, 11111);
	map.insert({3, 4}, {1}, 22222);

	check_interval_map(map,
		Value{{1, 2}, {2, 3}, 11111},
		Value{{2, 3}, {3, 4}, 0},
		Value{{3, 4}, {1}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(separate_intervals_different_values_6)
{
	map.insert({3, 4}, {1}, 11111);
	map.insert({1, 2}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{1, 2}, {2, 3}, 22222},
		Value{{2, 3}, {3, 4}, 0},
		Value{{3, 4}, {1}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(chained_intervals_same_value_1)
{
	map.insert({1, 3}, {1, 2}, 11111);
	map.insert({1, 2}, {2, 3}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(chained_intervals_same_value_2)
{
	map.insert({1, 2}, {2, 3}, 11111);
	map.insert({1, 3}, {1, 2}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(chained_intervals_same_value_3)
{
	map.insert({0}, {2, 7}, 11111);
	map.insert({2, 7}, {1}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(chained_intervals_same_value_4)
{
	map.insert({2, 7}, {1}, 11111);
	map.insert({0}, {2, 7}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(chained_intervals_different_values_1)
{
	map.insert({1, 3}, {1, 2}, 11111);
	map.insert({1, 2}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{1, 3}, {1, 2}, 11111},
		Value{{1, 2}, {2, 3}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(chained_intervals_different_values_2)
{
	map.insert({1, 2}, {2, 3}, 11111);
	map.insert({1, 3}, {1, 2}, 22222);

	check_interval_map(map,
		Value{{1, 3}, {1, 2}, 22222},
		Value{{1, 2}, {2, 3}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(chained_intervals_different_values_3)
{
	map.insert({0}, {2, 7}, 11111);
	map.insert({2, 7}, {1}, 22222);

	check_interval_map(map,
		Value{{0}, {2, 7}, 11111},
		Value{{2, 7}, {1}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(chained_intervals_different_values_4)
{
	map.insert({2, 7}, {1}, 11111);
	map.insert({0}, {2, 7}, 22222);

	check_interval_map(map,
		Value{{0}, {2, 7}, 22222},
		Value{{2, 7}, {1}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(common_start_point_same_value_1)
{
	map.insert({1, 3}, {1, 2}, 11111);
	map.insert({1, 3}, {2, 3}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(common_start_point_same_value_2)
{
	map.insert({1, 3}, {2, 3}, 11111);
	map.insert({1, 3}, {1, 2}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(common_start_point_different_values_1)
{
	map.insert({1, 3}, {1, 2}, 11111);
	map.insert({1, 3}, {2, 3}, 22222);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 22222});
}

BOOST_AUTO_TEST_CASE(common_start_point_different_values_2)
{
	map.insert({1, 3}, {2, 3}, 11111);
	map.insert({1, 3}, {1, 2}, 22222);

	check_interval_map(map,
		Value{{1, 3}, {1, 2}, 22222},
		Value{{1, 2}, {2, 3}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(common_end_point_same_value_1)
{
	map.insert({1, 3}, {2, 3}, 11111);
	map.insert({1, 2}, {2, 3}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(common_end_point_same_value_2)
{
	map.insert({1, 2}, {2, 3}, 11111);
	map.insert({1, 3}, {2, 3}, 11111);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(common_end_point_different_values_1)
{
	map.insert({1, 3}, {2, 3}, 11111);
	map.insert({1, 2}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{1, 3}, {1, 2}, 11111},
		Value{{1, 2}, {1, 3}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(common_end_point_different_values_2)
{
	map.insert({1, 2}, {2, 3}, 11111);
	map.insert({1, 3}, {2, 3}, 22222);

	check_interval_map(map, Value{{1, 3}, {2, 3}, 22222});
}

BOOST_AUTO_TEST_CASE(partial_overlap_same_value_1)
{
	map.insert({0}, {2, 3}, 11111);
	map.insert({1, 3}, {1}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(partial_overlap_same_value_2)
{
	map.insert({1, 3}, {1}, 11111);
	map.insert({0}, {2, 3}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(partial_overlap_different_values_1)
{
	map.insert({0}, {2, 3}, 11111);
	map.insert({1, 3}, {1}, 22222);

	check_interval_map(map,
		Value{{0}, {1, 3}, 11111},
		Value{{1, 3}, {1}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(partial_overlap_different_values_2)
{
	map.insert({1, 3}, {1}, 11111);
	map.insert({0}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{0}, {2, 3}, 22222},
		Value{{2, 3}, {1}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(interval_in_interval_same_value_1)
{
	map.insert({1, 4}, {3, 4}, 11111);
	map.insert({1, 3}, {2, 3}, 11111);

	check_interval_map(map, Value{{1, 4}, {3, 4}, 11111});
}

BOOST_AUTO_TEST_CASE(interval_in_interval_same_value_2)
{
	map.insert({1, 3}, {2, 3}, 11111);
	map.insert({1, 4}, {3, 4}, 11111);

	check_interval_map(map, Value{{1, 4}, {3, 4}, 11111});
}

BOOST_AUTO_TEST_CASE(interval_in_interval_different_values_1)
{
	map.insert({1, 4}, {3, 4}, 11111);
	map.insert({1, 3}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{1, 4}, {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 22222},
		Value{{2, 3}, {3, 4}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(interval_in_interval_different_values_2)
{
	map.insert({1, 3}, {2, 3}, 11111);
	map.insert({1, 4}, {3, 4}, 22222);

	check_interval_map(map, Value{{1, 4}, {3, 4}, 22222});
}

BOOST_AUTO_TEST_CASE(identical_intervals_same_value_1)
{
	map.insert({0}, {1}, 11111);
	map.insert({0}, {1}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(identical_intervals_same_value_2)
{
	map.insert({1, 4}, {3, 4}, 11111);
	map.insert({1, 3}, {2, 3}, 22222);
	map.insert({1, 3}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{1, 4}, {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 22222},
		Value{{2, 3}, {3, 4}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(identical_intervals_different_values_1)
{
	map.insert({0}, {1}, 11111);
	map.insert({0}, {1}, 22222);

	check_interval_map(map, Value{{0}, {1}, 22222});
}

BOOST_AUTO_TEST_CASE(identical_intervals_different_values_2)
{
	map.insert({1, 4}, {3, 4}, 11111);
	map.insert({1, 3}, {2, 3}, 22222);
	map.insert({1, 3}, {2, 3}, 11111);

	check_interval_map(map, Value{{1, 4}, {3, 4}, 11111});
}

BOOST_AUTO_TEST_CASE(identical_intervals_different_values_3)
{
	map.insert({1, 4}, {3, 4}, 11111);
	map.insert({1, 3}, {2, 3}, 22222);
	map.insert({1, 3}, {2, 3}, 33333);

	check_interval_map(map,
		Value{{1, 4}, {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 33333},
		Value{{2, 3}, {3, 4}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(length_zero_1)
{
	map.insert({1, 2}, {1, 2}, 11111);

	check_interval_map(map);
}

BOOST_AUTO_TEST_CASE(length_zero_2)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1, 2}, {1, 2}, 11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_zero_3)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1, 2}, {1, 2}, 22222);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_zero_4)
{
	map.insert({0}, {1, 2}, 11111);
	map.insert({0}, {0},    11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_zero_5)
{
	map.insert({0}, {1, 2}, 11111);
	map.insert({0}, {0},    11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_zero_6)
{
	map.insert({0},    {1},    11111);
	map.insert({1, 2}, {1, 2}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(length_zero_7)
{
	map.insert({0},    {1},    11111);
	map.insert({1, 2}, {1, 2}, 22222);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(length_zero_8)
{
	map.insert({1, 4}, {1, 2}, 11111);
	map.insert({1, 2}, {3, 4}, 22222);
	map.insert({1, 2}, {1, 2}, 22222);

	check_interval_map(map,
		Value{{1, 4}, {1, 2}, 11111},
		Value{{1, 2}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(length_zero_9)
{
	map.insert({1, 4}, {1, 2}, 11111);
	map.insert({1, 2}, {3, 4}, 22222);
	map.insert({1, 2}, {1, 2}, 11111);

	check_interval_map(map,
		Value{{1, 4}, {1, 2}, 11111},
		Value{{1, 2}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(length_zero_10)
{
	map.insert({1, 4}, {1, 2}, 11111);
	map.insert({1, 2}, {3, 4}, 22222);
	map.insert({1, 2}, {1, 2}, 33333);

	check_interval_map(map,
		Value{{1, 4}, {1, 2}, 11111},
		Value{{1, 2}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(length_negative_1)
{
	map.insert({1}, {1, 2}, 11111);

	check_interval_map(map);
}

BOOST_AUTO_TEST_CASE(length_negative_2)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1},    {1, 2}, 11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_negative_3)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1},    {1, 2}, 22222);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_negative_4)
{
	map.insert({0}, {1, 2}, 11111);
	map.insert({1}, {0},    11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_negative_5)
{
	map.insert({0}, {1, 2}, 11111);
	map.insert({1}, {0},    11111);

	check_interval_map(map, Value{{0}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(length_negative_6)
{
	map.insert({0},    {1},    11111);
	map.insert({1},    {1, 2}, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(length_negative_7)
{
	map.insert({0},    {1},    11111);
	map.insert({1},    {1, 2}, 22222);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(length_negative_8)
{
	map.insert({1, 4}, {1, 2}, 11111);
	map.insert({1, 2}, {3, 4}, 22222);
	map.insert({1},    {1, 2}, 22222);

	check_interval_map(map,
		Value{{1, 4}, {1, 2}, 11111},
		Value{{1, 2}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(length_negative_9)
{
	map.insert({1, 4}, {1, 2}, 11111);
	map.insert({1, 2}, {3, 4}, 22222);
	map.insert({1},    {1, 2}, 11111);

	check_interval_map(map,
		Value{{1, 4}, {1, 2}, 11111},
		Value{{1, 2}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(length_negative_10)
{
	map.insert({1, 4}, {1, 2}, 11111);
	map.insert({1, 2}, {3, 4}, 22222);
	map.insert({1},    {1, 2}, 33333);

	check_interval_map(map,
		Value{{1, 4}, {1, 2}, 11111},
		Value{{1, 2}, {3, 4}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(interval_over_crossing_1)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1, 2}, {1},    22222);
	map.insert({1, 3}, {2, 3}, 11111);

	check_interval_map(map,
		Value{{0}, {2, 3}, 11111},
		Value{{2, 3}, {1}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(interval_over_crossing_2)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1, 2}, {1},    22222);
	map.insert({1, 3}, {2, 3}, 22222);

	check_interval_map(map,
		Value{{0}, {1, 3}, 11111},
		Value{{1, 3}, {1}, 22222}
	);
}

BOOST_AUTO_TEST_CASE(interval_over_crossing_3)
{
	map.insert({0},    {1, 2}, 11111);
	map.insert({1, 2}, {1},    22222);
	map.insert({1, 3}, {2, 3}, 33333);

	check_interval_map(map,
		Value{{0},    {1, 3}, 11111},
		Value{{1, 3}, {2, 3}, 33333},
		Value{{2, 3}, {1},    22222}
	);
}

BOOST_AUTO_TEST_SUITE_END()

struct TestIntervalMapAndLineSegment : TestIntervalMap
{
	LineSegment segment = {{-4, -2}, {8, 4}};
};

BOOST_FIXTURE_TEST_SUITE(IntervalMapSegmentIntersectionTest, TestIntervalMapAndLineSegment)

BOOST_AUTO_TEST_CASE(segment_through_edges)
{
	Polygon polygon = {
		{2, -2},
		{-1, 1},
		{4, 1}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{2, 6}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(segment_through_vertices)
{
	Polygon polygon = {
		{-2, -1},
		{3, -2},
		{4, 2},
		{-1, 3}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{1, 6}, {2, 3}, 11111});
}

BOOST_AUTO_TEST_CASE(single_intersection_point)
{
	Polygon polygon = {
		{2, 1},
		{0, 2},
		{2, 3}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map);
}

BOOST_AUTO_TEST_CASE(collinear_intersection_1)
{
	Polygon polygon = {
		{6, 3},
		{-2, -1},
		{-1, 2}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{1, 6}, {5, 6}, 11111});
}

BOOST_AUTO_TEST_CASE(collinear_intersection_2)
{
	Polygon polygon = {
		{6, 3},
		{-2, -1},
		{2, 0}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{1, 6}, {5, 6}, 11111});
}

BOOST_AUTO_TEST_CASE(collinear_intersection_3)
{
	Polygon polygon = {
		{2, 1},
		{-2, -1},
		{2, 4},
		{5, 0}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{1, 6}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(collinear_intersection_4)
{
	Polygon polygon = {
		{-2, -1},
		{2, 1},
		{2, 4},
		{5, 0}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{1, 6}, {1, 2}, 11111});
}

BOOST_AUTO_TEST_CASE(two_intersections)
{
	Polygon polygon = {
		{1, -2},
		{-3, 1},
		{5, 0},
		{5, 4},
		{9, 0}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map,
		Value{{3, 12}, {5,  12}, 11111},
		Value{{5, 12}, {9,  12},     0},
		Value{{9, 12}, {10, 12}, 11111}
	);
}

BOOST_AUTO_TEST_CASE(start_inside)
{
	Polygon polygon = {
		{-4, 0},
		{-2, -3},
		{-6, -2}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{0}, {1, 12}, 11111});
}

BOOST_AUTO_TEST_CASE(end_inside)
{
	Polygon polygon = {
		{6, 0},
		{3, 3},
		{9, 5}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{2, 3}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(start_and_end_inside)
{
	Polygon polygon = {
		{-4,-4},
		{-6, 2},
		{10, 7},
		{10, 2},
		{2, 4},
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map,
		Value{{0},    {1,  5}, 11111},
		Value{{1, 5}, {5, 6},      0},
		Value{{5, 6}, {1},     11111}
	);
}

BOOST_AUTO_TEST_CASE(fully_inside)
{
	Polygon polygon = {
		{-4,-4},
		{-6, 2},
		{10, 7},
		{10, 2},
		{2, -4},
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map, Value{{0}, {1}, 11111});
}

BOOST_AUTO_TEST_CASE(fully_outside)
{
	Polygon polygon = {
		{10, 6},
		{-6, -2},
		{-4, -4},
		{10, 4},
		{-4, -6},
		{-10, 0},
		{12, 8},
		{14, 0}
	};

	map.insert_at_intersections(segment, polygon, 11111);

	check_interval_map(map);
}

BOOST_AUTO_TEST_SUITE_END()
