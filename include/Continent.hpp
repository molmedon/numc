#pragma once

#include <string>
#include <NuMC.hpp>
#include <Random.hpp>
#include <Vector3.hpp>
#include <readers/Bedmap.hpp>

namespace anita {

    // identifies a material at a given location in a continent
    enum class Material { Rock, Ice, Firn, Air, Water };

    // This class is used to represent a Continent (in our case, our Antarctica)
    // This call provides methods to query rock, ice, firn and other properties
    // of the continent at given locations and radii
    // default units are km and radians
    class Continent {

    public:
        // construct a new Continent. This loads BEDMAP2 and other data files
        Continent(): bedmap(readers::Bedmap()) {};

        // deconstructor; frees any allocated arrays and calls
        // deconstructor for Bedmap and the like
        ~Continent() {};

        // get the surface ice elevation/radius (in km) at a given theta/phi (radians)
        double getSurfaceElevation(const double theta, const double phi) const;

        // get the thickness of the ice (in km) at a given theta/phi (radians)
        double getIceThickness(const double theta, const double phi) const;

        // get the depth of the rock/ice boundary (in km) at a given theta/phi (radians)
        double getRockDepth(const double theta, const double phi) const;

        // return the material at a given theta/phi (radians) and radius (in km)
        Material getMaterial(const double theta, const double phi, const double radius) const;

        // returns the density (in XXXXX) of the continent a given theta/phi(radians) and radius (in km)
        double getDensity(const double theta, const double phi, const double radius) const;
        double getDensity(const SphericalCoordinate coord) const;

        // returns the density (in XXXXX) of the continent a given theta/phi(radians) and radius (in km)
        std::pair<double, Material> getDensityAndMaterial(const SphericalCoordinate coord) const;
        std::pair<double, Material> getDensityAndMaterial(const double theta, const double phi, const double radius) const;

        // get the slope of a given theta/phi (radians) location in spherical coordinates
        // returns dr, dtheta, dphi in a Vec3
        Vector3<double> getSurfaceSlope(const double theta, const double phi) const;

        // get a random surface point on the continent; this only generates points
        // below 60 degrees
        SphericalCoordinate getRandomSurfacePoint() const;

        // generate a random surface direction centered at the given location
        SphericalCoordinate getRandomSurfaceDirection(const SphericalCoordinate location) const;

    private:

        // instance of BEDMAP2 data class
        readers::Bedmap bedmap;

    protected:

    };

} // END: namespace anita

