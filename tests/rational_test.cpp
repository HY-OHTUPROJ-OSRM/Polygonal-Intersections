#include "rational.h"
#include "test_utils.h"
#include <boost/test/unit_test.hpp>

struct TestRationals
{
	Rational x = {2, 3};
	Rational y = {3, 4};
	Rational z = {4, 6};
};

BOOST_FIXTURE_TEST_SUITE(RationalTest, TestRationals)

BOOST_AUTO_TEST_CASE(addition)
{
	BOOST_CHECK_EQUAL((x + y).nom, 17);
	BOOST_CHECK_EQUAL((x + y).den, 12);
}

 BOOST_AUTO_TEST_CASE(substraction)
{
	BOOST_CHECK_EQUAL((x - y).nom, -1);
	BOOST_CHECK_EQUAL((x - y).den, 12);
}

BOOST_AUTO_TEST_CASE(multiplication)
{
	BOOST_CHECK_EQUAL((x * y).nom, 6);
	BOOST_CHECK_EQUAL((x * y).den, 12);
}

BOOST_AUTO_TEST_CASE(division)
{
	BOOST_CHECK_EQUAL((x / y).nom, 8);
	BOOST_CHECK_EQUAL((x / y).den, 9);
}

BOOST_AUTO_TEST_CASE(equal)                     { BOOST_TEST(  x == z ); }
BOOST_AUTO_TEST_CASE(not_equal)                 { BOOST_TEST(  x != y ); }
BOOST_AUTO_TEST_CASE(less_than)                 { BOOST_TEST(  x <  y ); }
BOOST_AUTO_TEST_CASE(not_less_than)             { BOOST_TEST(!(x <  z)); }
BOOST_AUTO_TEST_CASE(greater_than)              { BOOST_TEST(  y >  z ); }
BOOST_AUTO_TEST_CASE(not_greater_than)          { BOOST_TEST(!(x >  y)); }
BOOST_AUTO_TEST_CASE(less_than_or_equal)        { BOOST_TEST(  x <= y ); }
BOOST_AUTO_TEST_CASE(not_less_than_or_equal)    { BOOST_TEST(!(y <= z)); }
BOOST_AUTO_TEST_CASE(greater_than_or_equal)     { BOOST_TEST(  x >= z ); }
BOOST_AUTO_TEST_CASE(not_greater_than_or_equal) { BOOST_TEST(!(x >= y)); }

BOOST_AUTO_TEST_SUITE_END()
