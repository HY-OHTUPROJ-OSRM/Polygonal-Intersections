#include "vector.h"
#include "test_utils.h"
#include <boost/test/unit_test.hpp>

struct TestVectors
{
	Vector2 a = {2, 3};
	Vector2 b = {3, 4};
	Vector3 c = {4, 6, 2};
	Vector3 d = {2, -2, 6};
};

BOOST_FIXTURE_TEST_SUITE(VectorTest, TestVectors)

BOOST_AUTO_TEST_CASE(addition_1)
{
	Vector2 v = {5, 7};
	BOOST_CHECK_EQUAL(a + b, v);
}

BOOST_AUTO_TEST_CASE(addition_2)
{
	Vector3 v = {6, 4, 8};
	BOOST_CHECK_EQUAL(c + d, v);
}

BOOST_AUTO_TEST_CASE(subtraction_1)
{
	Vector2 v = {-1, -1};
	BOOST_CHECK_EQUAL(a - b, v);
}

BOOST_AUTO_TEST_CASE(subtraction_2)
{
	Vector3 v = {2, 8, -4};
	BOOST_CHECK_EQUAL(c - d, v);
}

BOOST_AUTO_TEST_CASE(dot_product)
{
	BOOST_CHECK_EQUAL(Dot(a, b), 18);
}

BOOST_AUTO_TEST_CASE(dot_product_2)
{
	BOOST_CHECK_EQUAL(Dot(a, {0, 0}), 0);
}

BOOST_AUTO_TEST_CASE(determinant_1)
{
	BOOST_CHECK_EQUAL(Det(a, b), -1);
}

BOOST_AUTO_TEST_CASE(determinant_2)
{
	BOOST_CHECK_EQUAL(Det(b, a), 1);
}

BOOST_AUTO_TEST_SUITE_END()
