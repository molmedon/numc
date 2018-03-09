#pragma once

#include <string>

namespace anita { namespace readers {


        class Bedmap {

        public:
            // Initialize a new Bedmap class and read in the data files
            Bedmap() : surface(readBedmapData(surface_file)),
                       icemask(readBedmapData(icemask_file)),
                       thickness(readBedmapData(thickness_file)) {};

            ~Bedmap();

            // get the surface ice elevation/radius (in km) at a given theta/phi (radians)
            double getSurfaceElevation(const double theta, const double phi) const;

            // get the thickness of the ice (in km) at a given theta/phi (radians)
            double getIceThickness(const double theta, const double phi) const;

            // get the depth of the rock/ice boundary (in km) at a given theta/phi (radians)
            double getRockDepth(const double theta, const double phi) const;

        private:

            // filenames for respective BEDMAP files
            const std::string surface_file = "bedmap2_surface.flt";
            const std::string icemask_file = "bedmap2_icemask_grounded_and_shelves.flt";
            const std::string thickness_file = "bedmap2_thickness.flt";

            // parameters of underlying BEDMAP dataset
            const int ncols = 6667;
            const int nrows = 6667;
            const float xllcorner = -3333.5; // km
            const float yllcorner = -3333.5; // km
            const float cellsize = 1; // km
            const float NODATA = -9999;

            // BEDMAP2 data of surface elevation
            // ncols*nrows block of contiguous values
            const float *surface;

            // BEDMAP2 data of icemask showing where there are ice measurements
            // ncols*nrows block of contiguous values
            const float *icemask;

            // BEDMAP2 data of ice thickness
            // ncols*nrows block of contiguous values
            const float *thickness;

            // convert theta/phi to pair of indicies into BEDMAP2 data
            __attribute__((hot)) inline std::pair<int, int> coordToBEDMAPIndices(const double theta, const double phi) const;

            // read a Bedmap data file into memory
            float* readBedmapData(std::string filename) const;

        };



    } // END: namespace readers
} // END: namespace anita
