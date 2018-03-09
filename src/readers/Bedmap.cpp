#include <string>
#include <fstream>
#include <iostream>
#include <NuMC.hpp>
#include <Utils.hpp>
#include <algorithm>
#include <Constants.hpp>
#include <readers/Bedmap.hpp>

using namespace anita::readers;

Bedmap::~Bedmap() {

    // we have to free any allocate BEDMAP2 data files
    delete[] this->surface;
    delete[] this->thickness;
    delete[] this->icemask;
}

float* Bedmap::readBedmapData(std::string filename) const {
    // this function allocates a sufficient amount of memory, reads the binary data
    // table of BEDMAP2 data found in filename and returns it to the caller

    // we then attempt to open the file
    std::string path = std::string(DATA_DIR) + std::string("/bedmap2_bin/");
    FILE* file = fopen((path + filename).c_str(), "rb");

    // if we cannot open bedmap, we exit with error code 1
    if (!file) {
        std::cerr << "Unable to find bedmap file (" << filename << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // verify length of file matches 4*ncols*nrows
    fseek(file, 0, SEEK_END);
    const long int length = ftell(file);
    rewind(file);
    if (length != 4*this->ncols*this->nrows) { // 32-bit/4-byte floats per element
        std::cerr << "BEDMAP2 data files do not meet specifications. Quitting..." << std::endl;
        throw std::exception();
    }

    // allocate enough memory for data table
    float* _data = new float[this->ncols*this->nrows];

    // read the entire array into memory
    fread(_data, sizeof(float), static_cast<size_t>(length), file);

    // check that we read the entire file
    if (ftell(file) != 4*this->ncols*this->nrows) {
        std::cerr << "Unable to load all of BEDMAP2 file (" << filename << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // and we can now close the file
    fclose(file);

    // and return the data back to the caller
    return _data;

}

__attribute__((hot)) inline std::pair<int, int> Bedmap::coordToBEDMAPIndices(const double theta, const double phi) const {
    // Uses "Map Projections - A Working Manual" by J.P Snyder" https://pubs.usgs.gov/pp/1395/report.pdf
    // Stereographic Projections - starting pg. 154. Numerical example on pg. 315
    // IceMC version is icemodel.cc:L1122

    // compute t for this theta
    const double t0 = (1 - sin(theta))/(1 + sin(theta)); // used to calculate TC
    const double t1 = (1 + EARTH_E*sin(theta))/(1 - EARTH_E*sin(theta)); // used to calculate TC
    const double t = sqrt(tc0*pow(tc1, EARTH_E));

    // compute m
    const double m = cos(theta)/sqrt(1 - EARTH_E*EARTH_E*sin(theta)*sin(theta));

    // we use m, t to compute p
    const double p = t*(EARTH_A*M_C/T_C);

    // and then we can use p to find x, y
    const double x = p*sin(phi - PI/2); // reference to y-axis
    const double y = -p*cos(phi - PI/2);

    // and then convert these into indices
    int xi = static_cast<int>(std::round((x - this->xllcorner)/this->cellsize));
    int yi = static_cast<int>(std::round((-y - this->yllcorner)/this->cellsize));

    // clamp the values to the ncols/nrows range provided by the file
    // xi = utils::clamp(xi, 0, this->ncols);
    // yi = utils::clamp(yi, 0, this->nrows);

    // and make a pair
    return std::make_pair(xi, yi);

}


// get the surface ice elevation/radius (in km) at a given theta/phi (radians)
double Bedmap::getSurfaceElevation(const double theta, const double phi) const {

    // get the correct index
    std::pair<int, int> idx = coordToBEDMAPIndices(theta, phi);

    // and return the value indexed into the 2D chunk of contiguous memory
    return static_cast<float>(this->surface[idx.first*this->nrows + idx.second]);
}


// get the thickness of the ice (in km) at a given theta/phi (radians)
double Bedmap::getIceThickness(const double theta, const double phi) const {

    // get the correct index
    std::pair<int, int> idx = coordToBEDMAPIndices(theta, phi);

    // and return the value indexed into the 2D chunk of contiguous memory
    return static_cast<double>(this->thickness[idx.first*this->nrows + idx.second]);
}


// get the depth of the rock/ice boundary (in km) at a given theta/phi (radians)
double Bedmap::getRockDepth(const double theta, const double phi) const {

    // get the correct index
    std::pair<int, int> idx = coordToBEDMAPIndices(theta, phi);

    // get the surface elevation
    double surface_elevation = this->getSurfaceElevation(theta, phi);

    // get the ice thickness
    double ice_thickness = this->getIceThickness(theta, phi);

    // and return the value indexed into the 2D chunk of contiguous memory
    return surface_elevation - ice_thickness;

}
