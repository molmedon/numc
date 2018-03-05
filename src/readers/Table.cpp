// //////////////////////////////////////////////////////////////////////
// //  table class - loads a table from a file and provides various    //
// //  reading utilities                                               //
// //////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <algorithm>
#include <Random.hpp>
#include <readers/Table.hpp>

using namespace anita::readers;

void YTable::readYTableFromFile(std::string filename) {

    // try and open the file
    std::ifstream tablefile(filename);

    // right now we quit if we can't open the file so the caller
    // doesn't have to handle it as YTable's are normally essential
    // to simulation execution. This might probably change in the future
    if (!tablefile) {
        std::cerr << "Unable to create YTable from (" << filename
                  << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // we assume the format of the first line of the file is the same as NuTauSim
    // [number of final states per energy] [dimensions of each final state] [energy steps] [Emin] [Emax]
    // see header file for more info
    tablefile >> this->nfinal;
    tablefile >> this->ndim;
    tablefile >> this->imax;
    tablefile >> this->emin;
    tablefile >> this->emax;

    // we have read in the header
    // we now allocate memory and read in the data
    // 3D array [imax][nfinal][ndim]
    this->data = new double[this->imax*this->nfinal*this->ndim];

    // loop over all possible indices and save array into file in C order
    for (int i = 0; i < this->imax; i++) {
        for (int j = 0; j < this->nfinal; j++) {
            for (int k = 0; k < this->ndim; k++) {
                tablefile >> this->data[i*this->nfinal*this->ndim + j*this->nfinal + k];
            }
        }
    }

    // check that nothing happened
    if (tablefile.fail()) {
        std::cerr << "Encountered an error reading YTable from (" << filename
             << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // and close the file
    tablefile.close();

    return;
}

std::vector<double> YTable::evaluate(const double energy) const {

    // linear interpolation plus rounding to find the desired energy bin
    int idx = static_cast<int>(std::round((energy - this->emin)/
                                          (this->emax - this->emin)*(this->imax - 1)));

    // and clamp to the range of possible indices
    idx = std::clamp(idx, 0, this->imax - 1);

    // memory to store results
    std::vector<double> final;
    final.resize(static_cast<long unsigned int>(this->ndim));

    // pick a random final entry
    int entry = uniformInt(0, this->nfinal);

    // and fill in the array
    for (int i=0; i < this->ndim; i++) {
        final[static_cast<long unsigned int>(i)] = this->data[idx*this->nfinal*this->ndim + entry*this->nfinal + i];
    }

    // and we are done
    return final;
}
