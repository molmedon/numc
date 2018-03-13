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
    constexpr double EARTH_E = 0.08181919; // ellipticity
    constexpr double EARTH_A = 6378137./1000; // semi-major axis in km
    constexpr double EARTH_F = 1./298.257223563; // flattening of Earth ellipsoid
    constexpr double EARTH_B = EARTH_A*(1 - EARTH_F); // semi-minor axis in km

}
