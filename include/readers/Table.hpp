#pragma once

#include <string>
#include <fstream>

#include <NuMC.hpp>


namespace anita { namespace readers {


    // This class represents a general-purpose text 2D data table; it provides
    // loading and interpolated evaluation of the data at given values
    class Table {

    public:
        // Initialize a new Table and read the file located at 'filename'
        // into the Table
        Table(std::string filename) : data(readTableFromFile(filename)) {};

        // All we need to do is delete the allocated data
        ~Table() { delete[] this->data; };

        // evaluate the table data at a given (x, y) position
        // using a cubic spline interpolant
        double evaluate(double x, double y);

    private:
        // parameters of the data table
        int imax, jmax;        // size of data array will be [imax][jmax]
        double xmin, xmax;     // span of data value range along i's
        double ymin, ymax;     // span of data value range along j's
        double* data;          // contiguous memory of [imax][jmax]

        // Read the file located at 'filename' and returned it to the caller
        // Expects the format:
        // imax jmax xmin xmax ymin ymax
        // data data data data data data data data ....
        // .... .... .... .... .... .... .... .... ....
        double* readTableFromFile(std::string filename);

    }; // END: Class Table


    // This class is used to load the final states data tables
    // from https://github.com/harmscho/NuTauSim. This data table
    // parameterizes the final energy of different CC and NC interactions,
    // i.e. Bjorken y-factor
    class YTable {

    public:

        // Construct a YTable and read it into memory
        YTable(std::string filename) { readYTableFromFile(filename); };

        // the only thing we need to do is free the table data
        ~YTable() { delete[] this->data; };

        // evaluate the YTable data at a given energy
        // energy in log10 eV
        std::vector<double> evaluate(const double energy) const;

    private:
        int       imax;      // number of steps in energy
        int       nfinal;    // number of final states per energy
        int       ndim;      // number of dimensions at each [energy][states] - nominally 2
        double    emin;      // minimum energy in the data file
        double    emax;      // maximum energy in the data file - used with the above to build the energy steps
        double*   data;      // the table data

        // read the data table from the file and initialize the table parameters
        void readYTableFromFile(std::string filename);

    }; // END: class YTable

    } // END: namespace readers
} // END: namespace anita
