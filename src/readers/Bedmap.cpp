#include <math.h>
#include <string>
#include <tuple>
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
    delete[] this->bed;
    delete[] this->icemask;
    delete[] this->thickness;
    delete[] this->gl04c_to_wgs;
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
        std::cerr << "BEDMAP2 data file " << filename << " does not meet specifications. Quitting..." << std::endl;
        std::cerr << "Calculated length: " << length << " != Expected Length: " << 4*this->ncols*this->nrows << std::endl;
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

    // this should probably be done while reading in the file
    // but for now, loop over the array and set all -9999.0 to NaN's
    // so we can detect when we accidentally use NaN's in calculations
    // we don't care about NaN-hitting performance because we should NEVER
    // be operating on NaN's
    for (int i = 0; i < this->ncols*this->nrows; i++) {
        if (_data[i] < -9990.0)
            _data[i] = std::numeric_limits<float>::quiet_NaN();
    }

    // and return the data back to the caller
    return _data;

}


std::pair<double, double> Bedmap::coordToBEDMAPLocation(const double theta, const double phi) const {
    // Uses "Map Projections - A Working Manual" by J.P Snyder" https://pubs.usgs.gov/pp/1395/report.pdf
    // Stereographic Projections - starting pg. 154. Numerical example on pg. 315
    // All page and equation numbers refer to Snyder
    // IceMC version is icemodel.cc:L1122

    // convert (theta, phi) to (latitude, longitude)
    const double lat = (PI/2.) - theta;
    const double lon = phi;

    // Pg. 161, Eq. 15-9
    const double t = tan(PI/4 + lat/2)/pow( (1 - EARTH_E*sin(-lat))/(1 + EARTH_E*sin(-lat)), EARTH_E/2.);

    // we use t, and EARTH_A, T_C and M_C to compute p
    // the factor is precomputed in Constants.h
    const double p = t*AMTC;

    // and then we can use p to find x, y relative to 0 degrees east
    // these are in km relative to center of Bedmap2 grid
    const double x = -p*sin(-lon);
    const double y = p*cos(-lon);

    // quick check to make sure that we are within the bedmap zone
    if ((x < this->xllcorner) || (x > -this->xllcorner) || (y < this->yllcorner) || (y > -this->yllcorner)) {
        std::cerr << "Converting (" << lat << ", " << phi << ") to BEDMAP coordinates "
                  << "gives out-of-range values (" << x << ", " << y << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // and make a pair
    return std::make_pair(x, y);

}


// get the surface ice elevation/radius (in m) relative to WGS84 ellipsoid at a given lat/phi (radians)
double Bedmap::getSurfaceElevation(const double theta, const double phi) const {

    // get the floating point location in the grid (in km)
    std::pair<double, double> loc = coordToBEDMAPLocation(theta, phi);

    // return the bedmap surface elevation at the x,y point
    return this->getSurfaceElevationAtPoint(loc.first, loc.second);

}


// get the surface elevation/radius (in m) at a given (x,y) in BEDMAP coordinates (km)
double Bedmap::getSurfaceElevationAtPoint(const double x, const double y) const {

    // bilinear interpolate data (returns EIGEN-GL04C) and then add conversion to WGS84
    return interpData(this->surface, x, y) + interpData(this->gl04c_to_wgs, x, y);

}


// get the thickness of the ice (in m) at a given lat/phi (radians)
double Bedmap::getIceThickness(const double theta, const double phi) const {

    // get the floating point location in the grid (in km)
    std::pair<double, double> loc = coordToBEDMAPLocation(theta, phi);

    // return the bedmap ice elevation at the x,y point
    return this->getIceThicknessAtPoint(loc.first, loc.second);
}


// get the ice thickness (in m) at a given (x,y) in BEDMAP coordinates (km)
double Bedmap::getIceThicknessAtPoint(const double x, const double y) const {

    // bilinear interpolate data (returns EIGEN-GL04C) and then add conversion to WGS84
    return this->interpData(this->thickness, x, y);
}


// get the radius of the rock/ice boundary (in m) at a given lat/phi (radians)
double Bedmap::getBedDepth(const double theta, const double phi) const {

    // get the floating point location in the grid (in km)
    std::pair<double, double> loc = coordToBEDMAPLocation(theta, phi);

    // return the bedmap ice elevation at the x,y point
    return this->getBedDepthAtPoint(loc.first, loc.second);

}

// get the radius of the rock/ice boundary (in m) at a given lat/phi (radians)
double Bedmap::getBedDepthAtPoint(const double x, const double y) const {

    // bilinear interpolate data (returns EIGEN-GL04C) and then add conversion to WGS84
    return this->interpData(this->bed, x, y) + interpData(this->gl04c_to_wgs, x, y);
}


// \brief Query BEDMAP2 icemask at a given theta,phi to identify valid data
// 0 = grounded, 1 = ice shelf, 127 = ocean
IceMask Bedmap::getIceMask(const double theta, const double phi) const {

    // get the floating point location in the grid (in km)
    std::pair<double, double> loc = coordToBEDMAPLocation(theta, phi);

    // and return the ice mask at this point
    return this->getIceMaskAtPoint(loc.first, loc.second);
}


// \brief Query BEDMAP2 icemask at a given theta,phi to identify valid data
// 0 = grounded, 1 = ice shelf, 127 = ocean
IceMask Bedmap::getIceMaskAtPoint(const double x, const double y) const {

    // evaluate the mask at this point
    const double mask = this->interpData(this->icemask, x, y);

    // if we are closer to being grounded, we return grounded
    if (mask < 1)
        return IceMask::Grounded;

    // we need to be conservative with the ice shelf since
    // points that don't have ice values are NaN and will corrupt
    // calculations using surfaceElevation
    if (mask == 1)
        return IceMask::IceShelf;

    // otherwise we assume we are in ocean
    return IceMask::Ocean;

}


// interpolate a function evaluated at the four points of a unit square to a point (x,y)
double Bedmap::interpIndex2D(const std::tuple<double, double, double, double> f,
                                    const std::pair<double, double> pos) const {

    // we interpolate on a unit square of indicies assuming
    // that the values of f are for the points (0, 0), (1, 0), (0, 1), (1, 1)
    const double x = pos.first; const double y = pos.second;

    // compute the bilinear interpolation
    return std::get<0>(f)*(1 - x)*(1 - y) + std::get<1>(f)*x*(1 - y) + std::get<2>(f)*(1 - x)*y + std::get<3>(f)*x*y;
}


// interpolate between data points to evaluate `data` at a given x,y in BEDMAP coordinates (km)
double Bedmap::interpData(const float *data, const double x, const double y) const {

    // find the index corresponding to x and y
    // we don't divide by cellsize since cellsize==1 for BEDMAP2 and division is "expensive"
    const double xi = abs(x - this->xllcorner) - 0.5;
    const double yi = abs(y + this->yllcorner) - 0.5;

    // get the data table values at this location and store in a tuple
    std::tuple<double, double, double, double> f;

    // assign values
    std::get<0>(f) = data[static_cast<int>(floor(yi)*this->ncols + floor(xi))];
    std::get<1>(f) = data[static_cast<int>(floor(yi)*this->ncols + ceil(xi))];
    std::get<2>(f) = data[static_cast<int>(ceil(yi)*this->ncols + floor(xi))];
    std::get<3>(f) = data[static_cast<int>(ceil(yi)*this->ncols + ceil(xi))];

    // interpolate in index space
    return interpIndex2D(f, std::pair<double, double>(fmod(xi, 1), fmod(yi, 1)));

}
