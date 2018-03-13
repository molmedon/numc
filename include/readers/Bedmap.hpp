#pragma once

#include <string>
#include <tuple>
#include <Constants.hpp>

namespace anita { namespace readers {

        // t_c. see Bedmap.cpp, used to convert to BEDMAP projection
        // this is assumes true scale at 71 degrees latitude
        // Uses "Map Projections - A Working Manual" by J.P Snyder" https://pubs.usgs.gov/pp/1395/report.pdf
        // Pg. 160

        // this is sin(71 deg), i.e. sin of the true scale latitude
        // this is phi_c in Snyder
        static constexpr double latc = -1.239183768915974;
        static constexpr double sinlatc = -0.9455185755993167;

        // this is t_c from Pg. 161, Eq. 21-34 in Snyder
        constexpr double T_C = tan(PI/4 + latc/2)/pow( (1 - EARTH_E*sin(-latc))/(1 + EARTH_E*sin(-latc)), EARTH_E/2.);

        // this is m_c from Pg. 161, Eq. 21-34 in Snyder
        // and computed using 14-15 on Pg. 160
        constexpr double M_C = cos(-latc)/sqrt(1 - pow(EARTH_E, 2)*pow((sin(-latc)), 2));

        // this is EARTH_A*M_C/T_C - this is the scale factor multiplied by t in coordinate transforms
        constexpr double AMTC = (EARTH_A*M_C)/T_C;

        ///
        /// \brief An enum representing the possible values of the Bedmap IceMask
        ///
        enum class IceMask { Grounded = 0, IceShelf = 1, Ocean = 127 };

        ///
        /// \brief A class providing read utilities for accessing Bedmap2 data
        ///
        /// Bedmap() objects load the required Bedmap2 data files on initialization
        /// and provide utilities to access Bedmap2 data at a given (lat, lon)
        ///
        class Bedmap {

        public:
            ///
            /// \brief Initialize a new Bedmap class and load all required data files
            ///
            Bedmap() : surface(readBedmapData(surface_file)),
                       bed(readBedmapData(bed_file)),
                       icemask(readBedmapData(icemask_file)),
                       thickness(readBedmapData(thickness_file)),
                       gl04c_to_wgs(readBedmapData(gl04c_to_wgs_file)) {};

            ///
            /// \brief Delete the memory allocated for Bedmap2 arrays
            ///
            ~Bedmap();

            ///
            /// \brief Get the surface elevation of ice (in m) relative to the WGS84 ellipsoid at a given (lat, lon)
            ///
            /// Use the BEDMAP2 surface_elevation data to compute the elevation of the ice surface relative
            /// to the WGS84 ellipsoid at a given latitude and longitude. This is only valid where
            /// the BEDMAP2 icemask is non-zero. Always check the icemask at lat/lon before calling this
            /// function. This returns NaN outside where the icemask is zero.
            ///
            double getSurfaceElevation(const double lat, const double lon) const;

            ///
            /// \brief Get surface elevation relative to the WGS84 ellipsoid (in m) at (x, y) (km) in Bedmap coordinates
            ///
            /// Use the BEDMAP2 surface_elevation data to compute the elevation of the ice surface (in m) relative
            /// to the WGS84 ellipsoid at a given x and y in polar stereographic coordrinates. This is only
            /// valid where the BEDMAP2 icemask is non-zero. Always check the icemask at lat/lon before calling
            /// this function. This returns NaN outside where the icemask is zero.
            ///
            double getSurfaceElevationAtPoint(const double x, const double y) const;

            ///
            /// \brief Get the thickness of ice (in m) at a (lat, lon) in radians
            ///
            /// Use the BEDMAP2 thickness data to compute the thickness of the ice (in m) at
            /// a given latitude and longitude. This is only
            /// valid where the BEDMAP2 icemask is non-zero. Always check the icemask at lat/lon before calling
            /// this function. This returns NaN outside where the icemask is zero.
            ///
            double getIceThickness(const double lat, const double lon) const;

            ///
            /// \brief Get ice thickness (in m) at (x, y) (km) location in Bedmap coordinates
            ///
            ///
            /// Use the BEDMAP2 thickness data to compute the thickness of the ice (in m) at
            /// a given x and y in polar stereographic coordrinates. This is only
            /// valid where the BEDMAP2 icemask is non-zero. Always check the icemask at lat/lon before calling
            /// this function. This returns NaN outside where the icemask is zero.
            ///
            double getIceThicknessAtPoint(const double x, const double y) const;

            ///
            /// \brief Get depth of rock bed (in km) at (lat,lon) in radians relative to WGS84 ellipsoid
            ///
            /// Use the BEDMAP2 depth data to compute the depth of the rock bed (in m)
            /// relative to the WGS84 ellipsoid at a given latitude and longitude
            ///
            double getBedDepth(const double lat, const double lon) const;

            ///
            /// \brief Get depth of rock bed (in km) at (x, y) (km) in Bedmap coordinatese relative to WGS84 ellipsoid
            ///
            /// Use the BEDMAP2 depth data to compute the depth of the rock bed (in m)
            /// relative to the WGS84 ellipsoid at a given x,y (in km) in polar stereographic coordinates
            ///
            double getBedDepthAtPoint(const double x, const double y) const;

            ///
            /// \brief Query BEDMAP2 icemask at a given lat,lon to identify valid data
            ///
            /// Query the BEDMAP2 icemask at a given latitude and longitude to determine
            /// the existence of an ice shelf at the index. This function is conservative
            /// and will return ocean if at least one point on the interpolated square is
            /// ocean, and will return grounded if at least one point on the interpolated
            /// square is grounded.
            ///
            /// 0 = grounded, 1 = ice shelf, 127 = ocean
            ///
            IceMask getIceMask(const double lat, const double lon) const;

            ///
            /// \brief Query BEDMAP2 icemask at a given lat,lon to identify valid data
            ///
            /// Query the BEDMAP2 icemask at a given latitude and longitude to determine
            /// the existence of an ice shelf at the index. This function is conservative
            /// and will return ocean if at least one point on the interpolated square is
            /// ocean, and will return grounded if at least one point on the interpolated
            /// square is grounded.
            ///
            /// 0 = grounded, 1 = ice shelf, 127 = ocean
            ///
            IceMask getIceMaskAtPoint(const double x, const double y) const;

        private:

            // filenames for respective BEDMAP files
            const std::string surface_file = "bedmap2_surface.flt";
            const std::string bed_file = "bedmap2_bed.flt";
            const std::string icemask_file = "bedmap2_icemask_grounded_and_shelves.flt";
            const std::string thickness_file = "bedmap2_thickness.flt";
            const std::string gl04c_to_wgs_file = "gl04c_geiod_to_wgs84.flt";

            // parameters of underlying BEDMAP dataset
            const int ncols = 6667;
            const int nrows = 6667;
            const float xllcorner = -3333.5; // km
            const float yllcorner = -3333.5; // km
            const float cellsize = 1; // km
            const float centeridx = (static_cast<float>(this->ncols) + 1.f)/2.f;
            const float NODATA = -9999;

            // BEDMAP2 data of surface elevation
            // ncols*nrows block of contiguous values
            const float *surface;

            // BEDMAP2 data of surface elevation
            // ncols*nrows block of contiguous values
            const float *bed;

            // BEDMAP2 data of icemask showing where there are ice measurements
            // ncols*nrows block of contiguous values
            const float *icemask;

            // BEDMAP2 data of ice thickness
            // ncols*nrows block of contiguous values
            const float *thickness;

            // BEDMAP2 data to convert from EIGEN-GL04C to WGS84
            // add this value to convert from EIGEN-GL04C
            const float *gl04c_to_wgs;

            ///
            /// \brief Convert (lat, lon) in radians to indices into BEDMAP2 data
            ///
            /// Converts a lat, lon location on the surface of the Earth to a (x, y)
            /// point (in km) reference to the center of the BEDMAP2 grid (lat: 0N, lon: 0E).
            /// Does not round to allow for later interpolation between grid cells
            ///
            inline std::pair<double, double> coordToBEDMAPLocation(const double lat, const double lon) const __attribute__((hot));

            ///
            /// \brief Access value in `data` at x,y locations (in km) using bilinear interpolation
            ///
            /// Given two locations in BEDMAP coordinates (x,y in km), use bilinear interpolation on
            /// the values of `data` and return the interpolated data value. Only valid on datasets
            /// that are the full size of BEDMAP (i.e. not Vostok or uncertainty data products)
            ///
            inline double interpData(const float *data, const double x, const double y) const;

            ///
            /// \brief Interpolate a function f evaluated on the unit square
            ///
            /// Interpolate a function assuming that the function is evaluated on
            /// on the unit square; `f` should be of length four and contain the elements
            /// evaluated at (0, 0), (1, 0), (0, 1), (1, 1), where `pos` is a position in
            /// the unit square to evaluate the function at.
            /// This function does NO error checking on the range of `pos`, or the number
            /// of elements in f
            ///
            inline double interpIndex2D(const std::tuple<double, double, double, double> f,
                                        const std::pair<double, double> pos) const __attribute__((hot));

            // read a Bedmap data file into memory
            float* readBedmapData(std::string filename) const;

        };

    } // END: namespace readers
} // END: namespace anita
