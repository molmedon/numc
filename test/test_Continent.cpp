#include <doctest.h>

#include <vector>
#include <numeric>
#include <iostream>
#include <Random.hpp>
#include <Constants.hpp>
#include <Continent.hpp>

TEST_SUITE_BEGIN("continent");

TEST_CASE("CREATE CONTINENT") {

    const anita::Continent continent = anita::Continent();
}

TEST_CASE("BASIC METHODS") {

    const anita::Continent continent = anita::Continent();

    // test the WGS84 radius routine
    SUBCASE("RADIUS") {

        // these should give semi major radius
        CHECK(continent.getEarthRadius(anita::degToRad(-90)) == anita::EARTH_A);
        CHECK(continent.getEarthRadius(anita::degToRad(90)) == anita::EARTH_A);

        // these should give semi minor radius
        CHECK(continent.getEarthRadius(anita::degToRad(0)) == anita::EARTH_B);

        // if we average across lots of latitudes, it should equal average radius
        double sum = 0; int N = 10000;
        for (int i = 0; i < N; i++) {
            // draw a random latitude and add it to the accumulator
            // uses true spherical point picking
            // see http://mathworld.wolfram.com/SpherePointPicking.html
            double latitude = acos(2*uniform() - 1);
            sum += continent.getEarthRadius(latitude);
        }
        // and check that the average is approximately the known average radius
        CHECK(sum/N == doctest::Approx(6371.0088));
    }

    // test that our random surface point picking is getting good coverage
    SUBCASE("RANDOM SURFACE") {

        // we want to check for a sample of 10,000
        unsigned int N = 10000;

        // create some vectors
        std::vector<double> thetas;
        std::vector<double> phis;

        // reserve space in the vectors
        thetas.reserve(N);
        phis.reserve(N);

        // iterate and add to the vector
        for (unsigned int i = 0; i < N; i++) {
            auto point = continent.getRandomSurfacePoint();
            thetas.push_back(point.theta);
            phis.push_back(point.phi);
        }

        // compute min and max values of theta
        double min_theta = (*std::min_element(thetas.begin(), thetas.end()));
        double max_theta = (*std::max_element(thetas.begin(), thetas.end()));

        // we only wants point on the spherical cap below -60 degrees
        // check that min value of theta is > -90
        CHECK(min_theta >= anita::degToRad(150));
        // and that the max value is < -60
        CHECK(max_theta <= anita::degToRad(180));

        // we draw phi from [0, 2*pi) so check the min and max
        const double min_phi = (*std::min_element(phis.begin(), phis.end()));
        const double max_phi = (*std::max_element(phis.begin(), phis.end()));
        CHECK(min_phi >= 0);
        CHECK(max_phi < 2*anita::PI);

        // check that the mean is approximately pi
        const double mean_phi = std::accumulate(phis.begin(), phis.end(), 0.)/static_cast<double>(N);
        CHECK(mean_phi == doctest::Approx(anita::PI).epsilon(0.05));

    }

}


TEST_SUITE_END();
