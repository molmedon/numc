#pragma once

#include <Constants.hpp>

// defines a TODO pragma to produce compile time notes
#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))

namespace anita {

    struct SphericalCoordinate {
        double theta;
        double phi;
        double r;

        // constructors
        // zero everything out
        SphericalCoordinate() : theta(0), phi(0), r(0) {};

        // or particular values
        SphericalCoordinate(const double t, const double p, const double R) : theta(t), phi(p), r(R) {};


        ~SphericalCoordinate() {};
    };


} // END: namespace anita
