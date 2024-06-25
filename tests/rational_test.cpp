#include "rational.h"
#include "test_utils.h"
#include <boost/test/unit_test.hpp>

struct TestRationals
{
	Rational x = {2, 3};
	Rational y = {3, 4};
	Rational z = {4, 6};
	Rational w = {5, -2};
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
BOOST_AUTO_TEST_CASE(less_than_1)               { BOOST_TEST(  x <  y ); }
BOOST_AUTO_TEST_CASE(less_than_2)               { BOOST_TEST(  w <  x ); }
BOOST_AUTO_TEST_CASE(not_less_than)             { BOOST_TEST(!(x <  z)); }
BOOST_AUTO_TEST_CASE(greater_than_1)            { BOOST_TEST(  y >  z ); }
BOOST_AUTO_TEST_CASE(greater_than_2)            { BOOST_TEST(  z >  w ); }
BOOST_AUTO_TEST_CASE(not_greater_than)          { BOOST_TEST(!(x >  y)); }
BOOST_AUTO_TEST_CASE(less_than_or_equal)        { BOOST_TEST(  x <= y ); }
BOOST_AUTO_TEST_CASE(not_less_than_or_equal)    { BOOST_TEST(!(y <= z)); }
BOOST_AUTO_TEST_CASE(greater_than_or_equal)     { BOOST_TEST(  x >= z ); }
BOOST_AUTO_TEST_CASE(not_greater_than_or_equal) { BOOST_TEST(!(x >= y)); }

BOOST_AUTO_TEST_CASE(is_zero_1) { BOOST_TEST(Rational{0}.is_zero()); }
BOOST_AUTO_TEST_CASE(is_zero_2) { BOOST_TEST(!x.is_zero()); }

BOOST_AUTO_TEST_CASE(conversion_1) { BOOST_CHECK_EQUAL(static_cast<double>(x), 2./3); }
BOOST_AUTO_TEST_CASE(conversion_2) { BOOST_CHECK_EQUAL(static_cast<int64_t>(x), 0); }

BOOST_AUTO_TEST_SUITE_END()
