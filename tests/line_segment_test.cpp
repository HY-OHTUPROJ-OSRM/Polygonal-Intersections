#include "line_segment.h"
#include "test_utils.h"
#include <boost/test/unit_test.hpp>

struct TestLineSegment
{
	LineSegment segment= {{1, 2}, {3, 4}};
};

BOOST_FIXTURE_TEST_SUITE(LineSegmentTest, TestLineSegment)

BOOST_AUTO_TEST_CASE(eval)
{
	BOOST_CHECK_EQUAL(segment.eval({2, 3}), Vector3(7, 10, 3));
}

BOOST_AUTO_TEST_CASE(contains_1)
{
	BOOST_TEST(segment.contains(segment.a));
}

BOOST_AUTO_TEST_CASE(contains_2)
{
	BOOST_TEST(segment.contains(segment.b));
}

BOOST_AUTO_TEST_CASE(contains_3)
{
	BOOST_TEST(segment.contains({2, 3}));
}

BOOST_AUTO_TEST_CASE(contains_4)
{
	BOOST_TEST(!segment.contains({2, 4}));
}

BOOST_AUTO_TEST_CASE(contains_5)
{
	BOOST_TEST(!segment.contains({0, 2}));
}

BOOST_AUTO_TEST_CASE(contains_6)
{
	BOOST_TEST(!segment.contains({4, 4}));
}

BOOST_AUTO_TEST_CASE(contains_projection_1)
{
	BOOST_TEST(segment.contains_projection({2, 4}));
}

BOOST_AUTO_TEST_CASE(contains_projection_2)
{
	BOOST_TEST(!segment.contains_projection({5, 9}));
}

BOOST_AUTO_TEST_CASE(contains_projection_empty_segment_1)
{
	BOOST_TEST(LineSegment({1,2}, {1,2}).contains_projection({1, 2}));
}

BOOST_AUTO_TEST_CASE(contains_projection_empty_segment_2)
{
	BOOST_TEST(!LineSegment({1,2}, {1,2}).contains_projection({2, 4}));
}

BOOST_AUTO_TEST_CASE(find_intersection_parellel)
{
	auto intersection = find_intersection(segment, {{2, 4}, {4, 6}});

	BOOST_TEST(std::holds_alternative<std::monostate>(intersection));
}

BOOST_AUTO_TEST_CASE(find_intersection_collinear_1)
{
	auto intersection = find_intersection(segment, {{0, 1}, {5, 6}});

	BOOST_TEST(std::holds_alternative<RationalInterval>(intersection));
}

BOOST_AUTO_TEST_CASE(find_intersection_collinear_2)
{
	auto intersection = find_intersection(segment, {{2, 3}, {9, 10}});

	BOOST_TEST(std::holds_alternative<RationalInterval>(intersection));
}

BOOST_AUTO_TEST_CASE(find_intersection_collinear_3)
{
	auto intersection = find_intersection(segment, {{4, 5}, {9, 10}});

	BOOST_TEST(std::holds_alternative<std::monostate>(intersection));
}

BOOST_AUTO_TEST_CASE(find_intersection_collinear_4)
{
	auto intersection = find_intersection(segment, {{9, 10}, {2, 3}});

	BOOST_TEST(std::holds_alternative<RationalInterval>(intersection));
}

BOOST_AUTO_TEST_CASE(length)
{
	BOOST_CHECK_CLOSE(segment.length(), 2.8284271247461900976, 1e-9);
}

BOOST_AUTO_TEST_SUITE_END()
