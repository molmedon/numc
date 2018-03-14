#pragma once

#include <string>
#include <NuMC.hpp>
#include <Random.hpp>
#include <Vector3.hpp>
#include <readers/Earth.hpp>
#include <readers/Bedmap.hpp>

namespace anita {

    ///
    /// \brief Represents various different materials
    ///
    enum class Material { Rock, Ice, Firn, Air, Ocean, FreshWater };

    ///
    /// \brief A class representing a spherical coordinate
    ///
    struct SphericalCoordinate {

        double theta; ///< theta as measured from the North Pole southwards
        double phi; ///< phi is measured azimuthally from the prime meridian
        double r; ///< radius (try and stick to kilometers when you can)

        ///
        /// \brief Construct a new SphericalCoordinate with all components
        ///
        SphericalCoordinate() : theta(0), phi(0), r(0) {};

        ///
        /// \brief Construct a new SphericalCoordinate with the specified values
        ///
        SphericalCoordinate(const double t, const double p, const double R) : theta(t), phi(p), r(R) {};

        ~SphericalCoordinate() {};
    };

    ///
    /// \brief Represents the continent of Antarctica and all Earth-related values
    ///
    /// This class wraps all available low level information (such as BEDMAP, PREM, etc.)
    /// and provides uniform methods to access such information. This is the only class
    /// that should be used to access Earth-related information.
    ///
    /// Unless otherwise stated, distance units are in km, density is \f$g/cm^{3}\f$
    /// and angles are in radians with theta measured from the North Pole, and
    /// phi azimuthally from the prime meridian (i.e. phi == lon)
    ///
    class Continent {

    public:
        ///
        /// \brief Construct a new Continent object, loading all necessary data files
        ///
        Continent(): bedmap(), earth() {};
        ~Continent() {};

        ///
        /// \brief Get the radius (in km) of the surface (ice, ocean, rock, etc.) at a theta/phi (radians)
        ///
        double getSurfaceElevation(const double theta, const double phi) const;

        ///
        /// \brief Get the thickness of the ice (in km) at a given theta/phi (radians)
        ///
        double getIceThickness(const double theta, const double phi) const;

        ///
        /// \brief Get the radius of the rock bed (in km) at a given theta/phi (radians)
        ///
        double getBedRadius(const double theta, const double phi) const;

        ///
        /// \brief Get the material at a given theta/phi (radians) and radius (in km)
        ///
        Material getMaterial(const double theta, const double phi, const double radius) const;

        ///
        /// \brief Get the density in g/cm^3 at a given theta/phi (radians) and radius (in km)
        ///
        double getDensity(const double theta, const double phi, const double radius) const;

        ///
        /// \brief Get the density in g/cm^3 at a given spherical coordinate
        ///
        double getDensity(const SphericalCoordinate coord) const;

        ///
        /// \brief Get the density (in g/cm^3) and material at a given theta/phi (radians) and r (km)
        ///
        std::pair<double, Material> getDensityAndMaterial(const double theta, const double phi, const double radius) const;

        ///
        /// \brief Get the density (in g/cm^3) and material at a given spherical coordinate
        ///
        std::pair<double, Material> getDensityAndMaterial(const SphericalCoordinate coord) const;

        ///
        /// \brief Get the slope (units?) of the surface at a given theta/phi (radians)
        ///
        Vector3<double> getSurfaceSlope(const double theta, const double phi) const __attribute__((deprecated)) {
            return Vector3<double>(0, 0, 0); };

        ///
        /// \brief Get a random point on the surface of the Earth below -60d latitude
        ///
        SphericalCoordinate getRandomSurfacePoint() const;

        ///
        /// \brief Get a random unit vector direction in spherical coordinates
        ///
        SphericalCoordinate getRandomSurfaceDirection() const;

        ///
        /// \brief Get radius (in km) of WGS84 ellipsoid at a given theta (from North Pole)
        ///
        /// Assuming that the Earth is a perfect ellipsoid, use the WGS84
        /// major and minor radii to compute the WGS84 ellipsoid radius at
        /// a given thetaitude
        ///
        /// Uses \f$R = \frac{ab}{\sqrt{a^2 \sin^2(\theta) + b^2 \cos^2(\theta)}}\f$
        /// with \f$a\f$ given by WGS84 and \f$b = (1 - f)a\f$ with \f$f\f$ given by WGS84
        ///
        double getEarthRadius(const double theta) const;

        ///
        /// \brief Uses getEarthRadius to compute radius (in km) of WGS84 ellipsoid at a given latitude
        ///
        double getEarthRadiusLat(const double lat) const { return this->getEarthRadius(PI - lat); };

    private:

        // instance of BEDMAP data class to provide access to BEDMAP2 data
        const readers::Bedmap bedmap;

        // instance of Earth class to access PREM density data
        const readers::Earth earth;

    protected:

    };

} // END: namespace anita

