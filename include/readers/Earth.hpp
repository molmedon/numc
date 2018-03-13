#pragma once

#include <map>
#include <Math/Interpolator.h>

namespace anita { namespace readers {

        ///
        /// \brief Provide access to Preliminary Reference Earth Model densities
        ///
        /// This class uses linear interpolation to estimate the density of the Earth
        /// in (g/cm^3) at a given radius (in km) using data from the Preliminary
        /// Earth Reference Model.
        ///
        /// See the below links for more information:
        /// https://www.cfa.harvard.edu/~lzeng/papers/PREM.pdf
        /// http://ds.iris.edu/ds/products/emc-prem/
        ///
        class Earth {

        public:
            ///
            /// \brief Get the density in g/cm^3 at a given radius R (in km)
            ///
            /// Use the polynomial fits from the Preliminary Reference Earth Model (PREM500)
            /// to estimate the density at a given radius within the Earth. This function
            /// throws an exception if used to access radius outside of the specified
            /// domain of PREM.
            ///
            ///
            double getDensity(const double r) const;

            ///
            /// \brief The maximum radius (in km) that this model is valid
            ///
            const double max_radius = 6371;

            ///
            /// \brief The number of entries in this PREM model
            ///
            const int max_rindex = 199;

            ///
            /// \brief Construct a model of the Earth using the PREM datafile at this->filename
            ///
            /// This function loads the specified PREM datafile, which is assumed to meet
            /// the following specification by column
            /// radius,depth,density,Vpv,Vph,Vsv,Vsh,eta,Q-mu,Q-kappa
            ///
            Earth() : data(readPREMFile()) {};

            ///
            /// \brief Free initialized spline objects
            ///
            ~Earth() {};

        private:

            // filename in data dir containing the model
            const std::string filename = "PREM_1s.csv";

            // a pair of vectors - of radii and density
            const std::pair<std::vector<double>, std::vector<double>> data;

            // read the PREM file into memory
            std::pair<std::vector<double>, std::vector<double>> readPREMFile() const;

        };

    } // END: namespace readers
} // END: namespace anita
