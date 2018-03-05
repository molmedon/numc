#pragma once

#include <math.h>

namespace anita {

    // mathematical and physics constants
    constexpr double PI = M_PI;
    constexpr double N_A = 6.0221415e23; // mol^-1

    // some simple utilities
    constexpr double degToRad(double deg) { return (deg/180.)*PI; }
    constexpr double radToDeg(double rad) { return (rad/PI)*180.; }

    // earth constants
    constexpr double EARTH_E = 0.0819919; // ellipticity
    constexpr double EARTH_A = 6378388/1000; // major axis in km

    // t_c. see Bedmap.cpp, used to convert to BEDMAP projection
    // this is assumes true scale at 71 degrees latitude
    // Uses "Map Projections - A Working Manual" by J.P Snyder" https://pubs.usgs.gov/pp/1395/report.pdf
    // Pg. 160
    const double refdeg = degToRad(71);
    const double tc0 = (1 - sin(refdeg))/(1 + sin(refdeg)); // used to calculate TC
    const double tc1 = (1 + EARTH_E*sin(refdeg))/(1 - EARTH_E*sin(refdeg)); // used to calculate TC
    const double T_C = sqrt(tc0*pow(tc1, EARTH_E));

    // m_c. see Bedmap.cpp, used to convert to BEDMAP projection
    // this is assumes true scale at 71 degrees latitude
    // Uses "Map Projections - A Working Manual" by J.P Snyder" https://pubs.usgs.gov/pp/1395/report.pdf
    // Pg. 159
    const double M_C = cos(refdeg)/sqrt(1 - EARTH_E*EARTH_E*sin(refdeg)*sin(refdeg));

    // define a quick lambda to return a value to the power of itself
    auto powToItself = [](double x) -> double {
        return pow(x, x);
    };

    // k_p is the true scale at the pole. see Bedmap.cpp, used to convert to BEDMAP projection
    // this is assumes true scale at 71 degrees latitude
    // Uses "Map Projections - A Working Manual" by J.P Snyder" https://pubs.usgs.gov/pp/1395/report.pdf
    // Pg. 159
    const double K_P = 0.5*M_C*(1./(EARTH_A*T_C))*sqrt( powToItself(1 + EARTH_E)*powToItself(1 - EARTH_E) );

}
