#include "polygon.h"
#include <boost/test/unit_test.hpp>

struct inits {
	Polygon testpoly1;
	void init_poly()
	{
		testpoly1.vertices = {
			Vector2_32{10, 10},
			Vector2_32{20, 10},
			Vector2_32{20, 20},
			Vector2_32{10, 20}
		};
	}
	inits() : testpoly1() {init_poly();}
};

BOOST_FIXTURE_TEST_SUITE( Multipolygon, inits )

BOOST_AUTO_TEST_CASE( multipolyContainsVector )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	Vector2 testvector = {35,35};
	BOOST_TEST(multipoly.contains(testvector));
}

BOOST_AUTO_TEST_CASE( multipolyDoesntContainVector )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};	
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	Vector2 testvector = {60,60};
	BOOST_TEST(!(multipoly.contains(testvector)));
}

BOOST_AUTO_TEST_CASE( multipolyEdgeIntersects )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	LineSegment testsegment = {Vector2{25,25},Vector2{35,40}};
	BOOST_TEST((multipoly.edge_intersects(testsegment)));
}

BOOST_AUTO_TEST_CASE( multipolyEdgeDoesntIntersect )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	LineSegment testsegment = {Vector2{25,5},Vector2{29,40}};
	BOOST_TEST(!(multipoly.edge_intersects(testsegment)));
}
/*
BOOST_AUTO_TEST_CASE( multipolyCountIntersectingSegmentsCrossing )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	int a = 0;
	PolygonalChain testchain;
	testchain.vertices = {
		Vector2_32{5, 15},
		Vector2_32{15, 15},
		Vector2_32{15, 35},
		Vector2_32{55, 35},
		Vector2_32{55, 55}
	};
	testchain.for_each_intersecting_segment(multipoly,[&](auto var){
		(void) var;
		a++;
	});
	BOOST_TEST(a==3);
}

BOOST_AUTO_TEST_CASE( multipolyCountIntersectingSegmentsNone )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	PolygonalChain testchain;
	testchain.vertices = {
		Vector2_32{5, 5},
		Vector2_32{5, 45},
		Vector2_32{45, 45},
		Vector2_32{55, 25},
		Vector2_32{20, 25}
	};
	int a = 0;
	testchain.for_each_intersecting_segment(multipoly,[&](auto var){
		(void) var;
		a++;
	});
	BOOST_TEST(a==0);
}

BOOST_AUTO_TEST_CASE( multipolyCountIntersectingSegmentsInside )
{
	Polygon testpoly2;
	testpoly2.vertices = {
		Vector2_32{30, 30},
		Vector2_32{40, 30},
		Vector2_32{40, 40},
		Vector2_32{30, 40}
	};
	MultiPolygon multipoly;
	multipoly.components = {testpoly1, testpoly2};
	PolygonalChain testchain;
	testchain.vertices = {
		Vector2_32{10, 15},
		Vector2_32{15, 15},
		Vector2_32{14, 16},
		Vector2_32{15, 18},
		Vector2_32{14, 19}
	};
	int a = 0;
	testchain.for_each_intersecting_segment(multipoly,[&](auto var){
		(void) var;
		a++;
	});
	BOOST_TEST(a==4);
}
*/
BOOST_AUTO_TEST_SUITE_END()
