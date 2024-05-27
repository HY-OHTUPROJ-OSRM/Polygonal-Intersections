#include "polygon.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( vectorOutsideArea )
{
    Polygon testpoly;
    testpoly.vertices = std::vector{
        Vector2_32{10,10},
        Vector2_32{20,10},
        Vector2_32{20,20},
        Vector2_32{10,20}
    };
    Vector2 testvector= {2,2};
    BOOST_TEST(!testpoly.contains(testvector));
}

BOOST_AUTO_TEST_CASE( vectorInsideArea )
{
    Polygon testpoly;
    testpoly.vertices = std::vector{
        Vector2_32{10,10},
        Vector2_32{20,10},
        Vector2_32{20,20},
        Vector2_32{10,20}
    };
    Vector2 testvector= {12,15};
    BOOST_TEST(testpoly.contains(testvector));
}

BOOST_AUTO_TEST_CASE( vectorAlongsideBorder )
{
    Polygon testpoly;
    testpoly.vertices = std::vector{
        Vector2_32{10,10},
        Vector2_32{20,10},
        Vector2_32{20,20},
        Vector2_32{10,20}
    };
    Vector2 testvector= {12,10};
    BOOST_TEST(testpoly.contains(testvector));
}


