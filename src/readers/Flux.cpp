#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <readers/Flux.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

using namespace anita::readers;

Flux::Flux(std::string filename) : spline(ROOT::Math::Interpolator(0, ROOT::Math::Interpolation::kCSPLINE)) {

    // directory of the flux data files
    const std::string flux_dir = std::string(DATA_DIR) + std::string("/fluxes/");
    const std::string flux_name = flux_dir+filename+std::string(".dat");

    // lets open the flux file
    std::ifstream influx(flux_name);

    // check that we opened the file correctly
    if (!influx.is_open()) {
        // should we default to particular flux model if we cannot find the flux file
        std::cerr << "Unable to open flux file (" << flux_name
                  << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // lets get the number of elements in the flux file
    int nlines = 0; influx >> nlines;

    // a map between energy and flux
    std::map<double, double> _flux_map;

    // variables to store energy and flux into
    double _energy = 0; double _flux = 0;
    for (int n = 0; n < nlines; n++) {

        // read energy and flux from the file
        influx >> _energy >> _flux;

        // convert GeV to eV and E2dN... -> EdN...
        // this is straight from IceMC - verify? RP
        _flux_map[_energy] = _flux + 9. - _energy;

    }

    // save the flux map into the class
    this->flux = _flux_map;

    // maps are guaranteed to be presorted
    //so the min and max energy is just the first and last element
    this->min_energy = this->flux.begin()->first;
    this->max_energy = this->flux.rbegin()->first;

    // and we find the maximum value in the map
    this->min_flux = (*std::min_element(this->flux.begin(),
                                        this->flux.end())).second;
    this->max_flux = (*std::max_element(this->flux.begin(),
                                       this->flux.end())).second;

    // we now build a cubic spline interpolant using ALGLIB
    // first, we need X and Y arrrays
    // get the keys of the flux data file that has been initialized
    std::vector<double> x;
    boost::copy(boost::adaptors::keys(this->flux),
                std::back_inserter(x));

    // and get the values
    std::vector<double> y;
    boost::copy(boost::adaptors::values(this->flux),
                std::back_inserter(y));

    // we build a interpolation scheme using ROOT; this is a cubic spline
    this->spline.SetData(static_cast<unsigned int>(x.size()), &x[0], &y[0]);

    // and we are done
}
