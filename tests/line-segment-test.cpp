#include "line_segment.h"
#include <boost/test/unit_test.hpp>

struct inits {
		LineSegment testsegment= {Vector2{1,2},Vector2{3,4}};
};

BOOST_FIXTURE_TEST_SUITE( LineSegments, inits )

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
		if(find_intersection(testsegment,testsegment2).has_value())
				{BOOST_FAIL("");}
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelCollinearProjection )
{
		LineSegment testsegment2= {Vector2{0,1},Vector2{5,6}};
		if(find_intersection(testsegment,testsegment2).has_value()){
				BOOST_TEST(find_intersection(testsegment,testsegment2).value().nom == 0);
				BOOST_TEST(find_intersection(testsegment,testsegment2).value().den == 1);
		} else
				{BOOST_FAIL("");}
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelColNoProjOverlap )
{
		LineSegment testsegment2= {Vector2{2,3},Vector2{9,10}};
		if(find_intersection(testsegment,testsegment2).has_value()){
				BOOST_TEST(find_intersection(testsegment,testsegment2).value().nom == 4);
				BOOST_TEST(find_intersection(testsegment,testsegment2).value().den == 8);
		} else
				{BOOST_FAIL("");}
	
}

BOOST_AUTO_TEST_CASE( findIntersectionParallelColNoProjNoOverlap )
{
		LineSegment testsegment2= {Vector2{4,5},Vector2{9,10}};
		if(find_intersection(testsegment,testsegment2).has_value())
				{BOOST_FAIL("");}
}

BOOST_AUTO_TEST_SUITE_END()
