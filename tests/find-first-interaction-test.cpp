#include "polygon.h"
#include <boost/test/unit_test.hpp>

struct inits {
    Polygon testpoly;
    PolygonalChain testchain;
    void init_poly()
    {
        testpoly.vertices = std::vector{
        Vector2{10,10},
        Vector2{20,10},
        Vector2{20,20},
        Vector2{10,20}
        };
    }
    inits() : testpoly() {init_poly();}
};

BOOST_FIXTURE_TEST_SUITE( FindFirstInteraction, inits )

BOOST_AUTO_TEST_CASE( segmentAlwaysOutside )
{
    testchain.vertices = std::vector{
        Vector2{1,3},
        Vector2{3,5},
        Vector2{5,7}
    };
    if(testchain.find_first_intersection(testpoly).has_value())
        {BOOST_FAIL("");}  
}

BOOST_AUTO_TEST_CASE( segmentAlwaysInside )
{
    testchain.vertices = std::vector{
        Vector2{12,13},
        Vector2{11,15},
        Vector2{12,11}
    };
    if(testchain.find_first_intersection(testpoly).has_value()){
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().x == 12);
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().y == 13);
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().z == 1);
    }     
    else
        {BOOST_FAIL("");} 
}

BOOST_AUTO_TEST_CASE( segmentCrossesOnce )
{
    testchain.vertices = std::vector{
        Vector2{1,3},
        Vector2{3,15},
        Vector2{13,15}
    };
    if(testchain.find_first_intersection(testpoly).has_value()){
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().x == -1000);
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().y == -1500);
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().z == -100);
    }     
    else
        {BOOST_FAIL("");}
}

BOOST_AUTO_TEST_CASE( segmentCrossesManyTimes )
{
    testchain.vertices = std::vector{
        Vector2{1,3},
        Vector2{3,15},
        Vector2{23,15}
    };
    if(testchain.find_first_intersection(testpoly).has_value()){
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().x == -2000);
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().y == -3000);
        BOOST_TEST(testchain.find_first_intersection(testpoly).value().z == -200);
    }    
    else
        {BOOST_FAIL("");}
}
BOOST_AUTO_TEST_SUITE_END()
