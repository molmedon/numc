#include <doctest.h>
#include <Continent.hpp>

TEST_SUITE_BEGIN("continent");

TEST_CASE("Creating a Continent") {

    const anita::Continent continent = anita::Continent();
}

TEST_CASE("Evaluating surface characteristics") {

    const anita::Continent continent = anita::Continent();

    SUBCASE("getRandomSurfacePoint") {
        continent.getRandomSurfacePoint();
    }

    SUBCASE("getRandomSurfaceDirection") {
    }

    SUBCASE("getSurfaceElevation") {
        continent.getSurfaceElevation(0, 0);
    }
}


TEST_SUITE_END();
