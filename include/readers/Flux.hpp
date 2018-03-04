#pragma once

#include <map>
#include <boost/range.hpp>
#include <Math/Interpolator.h>

namespace anita::readers {

    class Flux {

    public:

        // read a standard ANITA flux file into memory
        // and initialize the class with the required values
        // the first line contains the number of energy entries
        // the first column is log10 eV, the second column
        // is E^2 dNdE in eV cm^-2 s^-1 sr^-1
        // this enables sharing of flux files with IceMC
        Flux(std::string filename);

        // the minimum/maximum energy defined in the flux file
        double min_energy;
        double max_energy;

        // the minimum/maximum flux defined in the flux file
        double min_flux;
        double max_flux;

        // returns the flux at a given energy
        double getFlux(double energy) const {
            return this->spline.Eval(energy);
        };

    private:
        // map from energy to flux
        // log10 eV to eV cm^-2 s^-1 sr^-1
        std::map<double, double> flux;

        // cubic spline interpolant from energy to flux
        ROOT::Math::Interpolator spline;

    };

}
