#include "rational.h"
#include <boost/test/unit_test.hpp>


struct inits {
	Rational x = {2,3};
	Rational y = {3,4};
	Rational z = {4,6};
};

BOOST_FIXTURE_TEST_SUITE( Rationaltest, inits )

BOOST_AUTO_TEST_CASE( rationalAddition )
{
	BOOST_TEST((x+y).nom == 17);
	BOOST_TEST((x+y).den == 12);
}

 BOOST_AUTO_TEST_CASE( rationalSubstraction )
{
	BOOST_TEST((x-y).nom == -1);
	BOOST_TEST((x-y).den == 12);
}

BOOST_AUTO_TEST_CASE( rationalMultiplication )
{
	BOOST_TEST((x*y).nom == 6);
	BOOST_TEST((x*y).den == 12);
}

BOOST_AUTO_TEST_CASE( rationalDivision )
{
	BOOST_TEST((x/y).nom == 8);
	BOOST_TEST((x/y).den == 9);
}

BOOST_AUTO_TEST_CASE( rationalEqual )
{
	BOOST_TEST((x == y) == false);
	BOOST_TEST((x == z) == true);
}

BOOST_AUTO_TEST_CASE( rationalLessThan )
{
	BOOST_TEST((x < y) == true);
	BOOST_TEST((x < z) == false);
}

BOOST_AUTO_TEST_CASE( rationalGreaterThan )
{
	BOOST_TEST((x > y) == false);
	BOOST_TEST((x > z) == false);
}

BOOST_AUTO_TEST_CASE( rationalAtMost )
{
	BOOST_TEST((x <= y) == true);
	BOOST_TEST((x <= z) == true);
}

BOOST_AUTO_TEST_CASE( rationalAtLeast )
{
	BOOST_TEST((x >= y) == false);
	BOOST_TEST((x >= z) == true);
}

BOOST_AUTO_TEST_SUITE_END()
