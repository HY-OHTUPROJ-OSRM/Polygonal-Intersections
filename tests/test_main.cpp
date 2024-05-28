#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include <concepts>
#include "polygon.h"

static_assert(!std::same_as<Polygon, int>);

BOOST_AUTO_TEST_CASE(test_case1) {
	int x = 1;
	int y = 2;
	BOOST_TEST(x + y == 3);
}

BOOST_AUTO_TEST_CASE(test_case2) {
	std::string s = "Hello, world!";
	BOOST_TEST(s.substr(0, 5) == "Hello");
}
