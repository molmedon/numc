#include <Continent.hpp>
#include <readers/Bedmap.hpp>

using namespace anita;

SphericalCoordinate Continent::getRandomSurfacePoint() const {

    // we start with the randomly picked surface point above 60 degrees w.r.t south pole
    // 3D sphere point picking:  http://mathworld.wolfram.com/SpherePointPicking.html
    double theta = 2*PI*uniform(); // BEDMAP data is only available above 60 degrees
    double phi = acos(2*uniform() - 1); // and uniform in theta
    double r = this->getSurfaceElevation(theta, phi);
    return SphericalCoordinate(theta, phi, r);

}

// we generate a random ray direction centered at the given location
SphericalCoordinate Continent::getRandomSurfaceDirection(SphericalCoordinate location) const {

    // we generate a random ray direction centered at the above location
    // and transform it to be located at the above location - unit length since it's a vector
    double theta_d =- 2*PI*uniform();
    double phi_d = acos(2*uniform() - 1);
    return SphericalCoordinate(theta_d, phi_d, 1.);
}

double Continent::getSurfaceElevation(const double theta, const double phi) const {

    return this->bedmap.getSurfaceElevation(theta, phi);
}
