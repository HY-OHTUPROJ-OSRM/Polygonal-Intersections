#include "polygon.h"
#include <boost/test/unit_test.hpp>

struct inits
{
	Polygon testpoly;

	void init_poly()
	{
		testpoly.vertices = {
			Vector2_32{10, 10},
			Vector2_32{20, 10},
			Vector2_32{20, 20},
			Vector2_32{10, 20}
		};
	}

	inits() : testpoly() { init_poly(); }
};

BOOST_FIXTURE_TEST_SUITE( Contains, inits )

BOOST_AUTO_TEST_CASE( vector2OutsideArea )
{
	Vector2 testvector = {2, 2};
	BOOST_TEST(!testpoly.contains(testvector));
}

BOOST_AUTO_TEST_CASE( vector2InsideArea )
{
	Vector2 testvector = {12, 15};
	BOOST_TEST(testpoly.contains(testvector));
}

BOOST_AUTO_TEST_CASE( vector2AlongsideBorder )
{
	Vector2 testvector = {12, 10};
	BOOST_TEST(testpoly.contains(testvector));
}

BOOST_AUTO_TEST_SUITE_END()
