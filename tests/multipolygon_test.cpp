#include "polygon.h"
#include <boost/test/unit_test.hpp>

struct TestMultiPolygon
{
	MultiPolygon multipolygon =
	{
		{ {10, 10}, {20, 10}, {20, 20}, {10, 20} },
		{ {30, 30}, {40, 30}, {40, 40}, {30, 40} }
	};
};

BOOST_FIXTURE_TEST_SUITE(Multipolygon, TestMultiPolygon)

BOOST_AUTO_TEST_CASE(contains_1)
{
	BOOST_TEST(multipolygon.contains({35, 35}));
}

BOOST_AUTO_TEST_CASE(contains_2)
{
	BOOST_TEST(!multipolygon.contains({60, 60}));
}

BOOST_AUTO_TEST_CASE(edge_intersects_1)
{
	BOOST_TEST(multipolygon.edge_intersects({{25,25}, {35,40}}));
}

BOOST_AUTO_TEST_CASE(edge_intersects_2)
{
	BOOST_TEST(!multipolygon.edge_intersects({{25,5}, {29,40}}));
}

BOOST_AUTO_TEST_SUITE_END()
