#include "vector.h"
#include <boost/test/unit_test.hpp>


struct inits {
	Vector2 x2 = {2,3};
	Vector2 y2 = {3,4};
	Vector3 i3 = {4,6,2};
	Vector3 j3 = {2,-2,6};
};

BOOST_FIXTURE_TEST_SUITE( Vectortest, inits )

BOOST_AUTO_TEST_CASE( Vector2Addition )
{
	BOOST_TEST((x2+y2).x == 5);
	BOOST_TEST((x2+y2).y == 7);
}

BOOST_AUTO_TEST_CASE( Vector2Substraction )
{
	BOOST_TEST((x2-y2).x == -1);
	BOOST_TEST((x2-y2).y == -1);
}

BOOST_AUTO_TEST_CASE( Vector3Addition )
{
	BOOST_TEST((i3+j3).x == 6);
	BOOST_TEST((i3+j3).y == 4);
	BOOST_TEST((i3+j3).z == 8);
}

BOOST_AUTO_TEST_CASE( Vector3Substraction )
{
	BOOST_TEST((i3-j3).x == 2);
	BOOST_TEST((i3-j3).y == 8);
	BOOST_TEST((i3-j3).z == -4);
}

BOOST_AUTO_TEST_CASE( Vector2Dot )
{
	BOOST_TEST(Dot(x2,y2) == 18);
}

BOOST_AUTO_TEST_SUITE_END()
