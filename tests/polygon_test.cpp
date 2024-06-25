#include "polygon.h"
#include <boost/test/unit_test.hpp>

struct TestPolygon
{
	Polygon polygon = {
		{10, 4},
		{8, 6},
		{12, 10},
		{12, 12},
		{14, 14},
		{13, 14},
		{13, 15},
		{6, 16},
		{2, 12},
		{2, 4},
		{4, 4},
		{-1, 2},
		{-1, 6},
		{1, 6},
		{1, 8},
		{0, 8},
		{-8, -2},
		{6, 0}
	};
};

BOOST_FIXTURE_TEST_SUITE(ContainsTest, TestPolygon)

BOOST_AUTO_TEST_CASE(empty)
{
	BOOST_TEST(!Polygon{}.contains({0, 0}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_1)
{
	BOOST_TEST(!polygon.contains({1, -2}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_2)
{
	BOOST_TEST(!polygon.contains({-14, 0}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_3)
{
	BOOST_TEST(!polygon.contains({20, 5}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_1)
{
	BOOST_TEST(polygon.contains({0, 0}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_2)
{
	BOOST_TEST(!polygon.contains({7, 16}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_3)
{
	BOOST_TEST(!polygon.contains({1, 5}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_4)
{
	BOOST_TEST(polygon.contains({9, 4}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_1)
{
	BOOST_TEST(polygon.contains({8, 10}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_2)
{
	BOOST_TEST(!polygon.contains({8, 16}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_3)
{
	BOOST_TEST(!polygon.contains({-8, 1}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_4)
{
	BOOST_TEST(polygon.contains({4, 5}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_5)
{
	BOOST_TEST(!polygon.contains({10, 6}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_6)
{
	BOOST_TEST(polygon.contains({10, 10}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_7)
{
	BOOST_TEST(!polygon.contains({10, 16}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_8)
{
	BOOST_TEST(!polygon.contains({14, 16}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_9)
{
	BOOST_TEST(polygon.contains({6, 8}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_10)
{
	BOOST_TEST(!polygon.contains({6, 20}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_11)
{
	BOOST_TEST(!polygon.contains({0, 10}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertical_edge_1)
{
	BOOST_TEST(!polygon.contains({1, 9}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertical_edge_2)
{
	BOOST_TEST(!polygon.contains({2, 13}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertical_edge_3)
{
	BOOST_TEST(polygon.contains({-1, 6}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertical_edge_4)
{
	BOOST_TEST(!polygon.contains({-1, 8}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertical_edge_5)
{
	BOOST_TEST(!polygon.contains({13, 16}));
}

BOOST_AUTO_TEST_CASE(point_on_edge_1)
{
	BOOST_TEST(polygon.contains({4, 14}));
}

BOOST_AUTO_TEST_CASE(point_on_edge_2)
{
	BOOST_TEST(polygon.contains({6, 0}));
}

BOOST_AUTO_TEST_CASE(point_on_edge_3)
{
	BOOST_TEST(polygon.contains({8, 2}));
}

BOOST_AUTO_TEST_CASE(point_on_vertical_edge_1)
{
	BOOST_TEST(polygon.contains({2, 10}));
}

BOOST_AUTO_TEST_CASE(point_on_vertical_edge_2)
{
	BOOST_TEST(polygon.contains({-1, 5}));
}

BOOST_AUTO_TEST_CASE(point_on_vertical_edge_3)
{
	BOOST_TEST(polygon.contains({12, 11}));
}

BOOST_AUTO_TEST_CASE(point_on_vertex)
{
	for (auto&& vert : polygon.vertices)
		BOOST_TEST(polygon.contains(vert));
}

BOOST_AUTO_TEST_CASE(point_on_overlapping_vertical_edges)
{
	polygon.vertices.at(11).x = 2;
	polygon.vertices.at(12).x = 2;

	BOOST_TEST(polygon.contains({2, 7}));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(LimitOfContainsTest, TestPolygon)

BOOST_AUTO_TEST_CASE(empty)
{
	BOOST_TEST(!Polygon{}.limit_of_contains({0, 0}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_1)
{
	BOOST_TEST(!polygon.limit_of_contains({-6, 2}, {-1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_2)
{
	BOOST_TEST(!polygon.limit_of_contains({0, -2}, {0, -1}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_3)
{
	BOOST_TEST(!polygon.limit_of_contains({1, 4}, {1, 10}));
}

BOOST_AUTO_TEST_CASE(ray_never_hits_4)
{
	BOOST_TEST(!polygon.limit_of_contains({9, 6}, {3, 2}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_1)
{
	BOOST_TEST(polygon.limit_of_contains({8, 12}, {2, 1}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_2)
{
	BOOST_TEST(!polygon.limit_of_contains({0, 4}, {-1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_3)
{
	BOOST_TEST(polygon.limit_of_contains({4, 6}, {-3, -1}));
}

BOOST_AUTO_TEST_CASE(ray_through_edge_4)
{
	BOOST_TEST(!polygon.limit_of_contains({8, 1}, {1, 20}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_1)
{
	BOOST_TEST(!polygon.limit_of_contains({0, 5}, {1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_2)
{
	BOOST_TEST(polygon.limit_of_contains({5, 6}, {-1, -2}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_3)
{
	BOOST_TEST(polygon.limit_of_contains({6, 6}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_4)
{
	BOOST_TEST(polygon.limit_of_contains({8, 12}, {-1, 2}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_5)
{
	BOOST_TEST(!polygon.limit_of_contains({12, 2}, {-3, -1}));
}

BOOST_AUTO_TEST_CASE(ray_through_vertex_6)
{
	BOOST_TEST(!polygon.limit_of_contains({-10, 0}, {1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_1)
{
	BOOST_TEST(polygon.limit_of_contains({12, 14}, {-1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_2)
{
	BOOST_TEST(polygon.limit_of_contains({12, 14}, {0, -1}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_3)
{
	BOOST_TEST(polygon.limit_of_contains({11, 11}, {1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_4)
{
	BOOST_TEST(!polygon.limit_of_contains({-3, 6}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_5)
{
	BOOST_TEST(polygon.limit_of_contains({-6, 0}, {5, 2}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_6)
{
	BOOST_TEST(!polygon.limit_of_contains({-2, 8}, {1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_7)
{
	BOOST_TEST(!polygon.limit_of_contains({2, -1}, {0, 1}));
}

BOOST_AUTO_TEST_CASE(ray_through_collinear_edge_8)
{
	BOOST_TEST(!polygon.limit_of_contains({8, 18}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_1)
{
	BOOST_TEST(!polygon.limit_of_contains({3, 13}, {1, 2}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_2)
{
	BOOST_TEST(polygon.limit_of_contains({3, 13}, {2, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_3)
{
	BOOST_TEST(!polygon.limit_of_contains({-4, 3}, {0, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_4)
{
	BOOST_TEST(!polygon.limit_of_contains({-4, 3}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_5)
{
	BOOST_TEST(polygon.limit_of_contains({-4, 3}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_6)
{
	BOOST_TEST(polygon.limit_of_contains({-4, 3}, {-1, -2}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_7)
{
	BOOST_TEST(!polygon.limit_of_contains({9, 5}, {2, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_8)
{
	BOOST_TEST(!polygon.limit_of_contains({9, 5}, {-2, 3}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_9)
{
	BOOST_TEST(!polygon.limit_of_contains({9, 5}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_10)
{
	BOOST_TEST(polygon.limit_of_contains({9, 5}, {0, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_11)
{
	BOOST_TEST(polygon.limit_of_contains({2, 5}, {1, 100}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_12)
{
	BOOST_TEST(polygon.limit_of_contains({2, 6}, {1, -100}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_13)
{
	BOOST_TEST(!polygon.limit_of_contains({2, 7}, {-1, 100}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_edge_14)
{
	BOOST_TEST(!polygon.limit_of_contains({2, 8}, {-1, -100}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_1)
{
	BOOST_TEST(polygon.limit_of_contains({3, 13}, {1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_2)
{
	BOOST_TEST(polygon.limit_of_contains({3, 13}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_3)
{
	BOOST_TEST(polygon.limit_of_contains({-4, 3}, {4, 5}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_4)
{
	BOOST_TEST(polygon.limit_of_contains({-4, 3}, {-4, -5}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_5)
{
	BOOST_TEST(polygon.limit_of_contains({9, 5}, {1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_6)
{
	BOOST_TEST(polygon.limit_of_contains({9, 5}, {-1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_7)
{
	BOOST_TEST(polygon.limit_of_contains({2, 5}, {0, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_collinear_edge_8)
{
	BOOST_TEST(polygon.limit_of_contains({2, 5}, {0, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_1)
{
	BOOST_TEST(!polygon.limit_of_contains({10, 4}, {-2, 3}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_2)
{
	BOOST_TEST(!polygon.limit_of_contains({10, 4}, {-2, -3}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_3)
{
	BOOST_TEST(!polygon.limit_of_contains({10, 4}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_4)
{
	BOOST_TEST(polygon.limit_of_contains({10, 4}, {-1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_5)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_6)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {-1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_7)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {3, 2}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_8)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {-3, -2}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_9)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {-1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_10)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_11)
{
	BOOST_TEST(polygon.limit_of_contains({4, 4}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_12)
{
	BOOST_TEST(!polygon.limit_of_contains({4, 4}, {-3, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_13)
{
	BOOST_TEST(polygon.limit_of_contains({-8, -2}, {1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_14)
{
	BOOST_TEST(polygon.limit_of_contains({-8, -2}, {6, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_15)
{
	BOOST_TEST(!polygon.limit_of_contains({-8, -2}, {2, 3}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_16)
{
	BOOST_TEST(!polygon.limit_of_contains({-8, -2}, {1, 0}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_17)
{
	BOOST_TEST(!polygon.limit_of_contains({-8, -2}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_18)
{
	BOOST_TEST(!polygon.limit_of_contains({-8, -2}, {-1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_19)
{
	BOOST_TEST(polygon.limit_of_contains({2, 12}, {1, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_20)
{
	BOOST_TEST(!polygon.limit_of_contains({2, 12}, {-1, -1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_21)
{
	BOOST_TEST(!polygon.limit_of_contains({2, 12}, {0, 1}));
}

BOOST_AUTO_TEST_CASE(ray_starts_on_vertex_22)
{
	BOOST_TEST(polygon.limit_of_contains({2, 12}, {0, -1}));
}

BOOST_AUTO_TEST_SUITE_END()
