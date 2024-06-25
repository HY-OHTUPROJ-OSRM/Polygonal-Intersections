#include "line_segment.h"
#include <boost/test/unit_test.hpp>

struct TestLineSegments {
	LineSegment testsegment= {Vector2{1,2},Vector2{3,4}};
};

BOOST_FIXTURE_TEST_SUITE( LineSegmentTest, TestLineSegments )

BOOST_AUTO_TEST_CASE( segmentEval )
{
	Rational x = {2,3};
	BOOST_TEST(testsegment.eval(x).x == 7);
	BOOST_TEST(testsegment.eval(x).y == 10);
	BOOST_TEST(testsegment.eval(x).z == 3);
}

BOOST_AUTO_TEST_CASE( segmentProjection )
{
	Vector2 vect1 = {5,9};
	Vector2 vect2 = {2,4};
	BOOST_TEST(testsegment.contains_projection(vect1) == false);
	BOOST_TEST(testsegment.contains_projection(vect2) == true);
}

BOOST_AUTO_TEST_CASE( findIntersectionParallel )
{
	LineSegment testsegment2= {Vector2{2,4},Vector2{4,6}};

	if (find_intersection(testsegment,testsegment2).index())
		BOOST_FAIL("");
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelCollinearProjection )
{
	LineSegment testsegment2= {Vector2{0,1},Vector2{5,6}};

	auto intersection = find_intersection(testsegment,testsegment2);

	BOOST_TEST(std::holds_alternative<RationalInterval>(intersection));
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelColNoProjOverlap )
{
	LineSegment testsegment2= {Vector2{2,3},Vector2{9,10}};

	auto intersection = find_intersection(testsegment,testsegment2);

	BOOST_TEST(std::holds_alternative<RationalInterval>(intersection));
	
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelColNoProjNoOverlap )
{
	LineSegment testsegment2= {Vector2{4,5},Vector2{9,10}};

	if (find_intersection(testsegment,testsegment2).index())
		BOOST_FAIL("");
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelColNoProjOverlapB )
{
	LineSegment testsegment2= {Vector2{9,10},Vector2{2,3}};

	auto intersection = find_intersection(testsegment,testsegment2);

	BOOST_TEST(std::holds_alternative<RationalInterval>(intersection));
}

BOOST_AUTO_TEST_CASE( segmentProjectionDotSegment )
{
	LineSegment dots = {Vector2{1,2},Vector2{1,2}};
	Vector2 vect = {1,2};
	Vector2 vect2 = {2,4};
	BOOST_TEST(dots.contains_projection(vect) == true);
	BOOST_TEST(dots.contains_projection(vect2) == false);
}

BOOST_AUTO_TEST_SUITE_END()
