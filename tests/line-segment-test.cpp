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

BOOST_AUTO_TEST_CASE( findIntersection )
{
    BOOST_TEST(1==1);
}

BOOST_AUTO_TEST_SUITE_END()
