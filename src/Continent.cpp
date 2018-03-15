#include <math.h>
#include <Continent.hpp>
#include <readers/Bedmap.hpp>

using namespace anita;

SphericalCoordinate Continent::getRandomSurfacePoint() const {

    // we start with the randomly picked surface point above 60 degrees w.r.t south pole
    // 3D sphere point picking:  http://mathworld.wolfram.com/SpherePointPicking.html
    // we only want values between -60 and -90
    double theta = acos(-uniform(sqrt(3.)/2., 1));
    double phi = 2*PI*uniform(0, 1);
    double r = this->getSurfaceElevation(theta, phi);
    return SphericalCoordinate(theta, phi, 0);

}

// we generate a random spherical unit vector
SphericalCoordinate Continent::getRandomSurfaceDirection() const {

    // we generate a random ray direction and set the length=1
    double theta_d = 2*PI*uniform();
    double phi_d = acos(2*uniform() - 1);
    return SphericalCoordinate(theta_d, phi_d, 1.);
}

// return the elevation of the surface at a given (theta, phi)
double Continent::getSurfaceElevation(const double theta, const double phi) const {

    // we check the BEDMAP2 ice mask to see if there is ice at our location
    if (this->bedmap.getIceMask(theta, phi) != readers::IceMask::Ocean) {
        // we have ice, so return surface elevation of ice
        return this->bedmap.getSurfaceElevation(theta, phi);
    }
    else {
        // there is no ice. Just ocean, so we return the WGS84 ellipsoid
        return this->getEarthRadius(theta);
    }
}

std::pair<double, Material> Continent::getDensityAndMaterial(const SphericalCoordinate coord) const {

    return getDensityAndMaterial(coord.theta, coord.phi, coord.r);
}
std::pair<double, Material> Continent::getDensityAndMaterial(const double theta, const double phi, const double radius) const {
    // TODO
    return std::make_pair(0, Material::Ice);
}

double Continent::getDensity(const SphericalCoordinate coord) const {
    return getDensity(coord.theta, coord.phi, coord.r);
}

double Continent::getDensity(const double theta, const double phi, const double radius) const {
    // TODO
    return 0;
}


/// \brief Compute value of WGS84 ellipsoid at a given theta
double Continent::getEarthRadius(const double lat) const {

    // convert latitude to spherical coordinates and return
    const double theta = PI/2 - lat;
    return (EARTH_A*EARTH_B)/sqrt(pow(EARTH_A*sin(theta), 2) + pow(EARTH_B*cos(theta), 2));
}
