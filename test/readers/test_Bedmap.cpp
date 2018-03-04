#include <readers/Bedmap.hpp>
#include <doctest.h>


TEST_CASE("BEDMAP") {

    SUBCASE("Creating a new BEDMAP object...") {
        // create a new Bedmap object
        anita::readers::Bedmap _bedmap = anita::readers::Bedmap();
    }

    // create a new Bedmap object
    anita::readers::Bedmap bedmap = anita::readers::Bedmap();

    SUBCASE("Bedmap query methods run without throwing an exception") {
        bedmap.getIceThickness(0, 0);
        bedmap.getSurfaceElevation(0, 0);
        bedmap.getRockDepth(0, 0);
    }

    SUBCASE("Points at edge of map contain NODATA=-9999") {
        double theta = 40; double phi = 0;
        CHECK(bedmap.getIceThickness(theta, phi) == -9999);
        CHECK(bedmap.getSurfaceElevation(theta, phi) == -9999);
        CHECK(bedmap.getRockDepth(theta, phi) == -9999);
    }

    SUBCASE("Points in middle of map have valid data") {
        double theta = 0; double phi = 0;
        CHECK(bedmap.getIceThickness(theta, phi) == -9999);
        CHECK(bedmap.getSurfaceElevation(theta, phi) == -9999);
        CHECK(bedmap.getRockDepth(theta, phi) == -9999);
    }
}
