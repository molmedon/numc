#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Utils.hpp>
#include <readers/Earth.hpp>
#include <Math/Interpolator.h>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/spirit/include/qi.hpp>

using namespace anita::readers;

std::pair<std::vector<double>, std::vector<double>> Earth::readPREMFile() const {

    // directory of the flux data files
    const std::string prem_name = std::string(DATA_DIR) + this->filename;

    // lets open the flux file
    std::ifstream inprem(prem_name);

    // check that we opened the file correctly
    if (!inprem.is_open()) {
        // should we default to particular flux model if we cannot find the flux file
        std::cerr << "Unable to open PREM file (" << prem_name
                  << "). Quitting..." << std::endl;
        throw std::exception();
    }

    // namespace for boost parser
    namespace qi = boost::spirit::qi;

    // types for parsing
    using qi::double_;
    using qi::char_;

    // vector to store the values in for splining
    std::vector<double> density;
    std::vector<double> radii;

    // read the file line by line
    std::string line;
    while (std::getline(inprem, line)) {

        // start and end of string
        std::string::iterator begin = line.begin();
        std::string::iterator end = line.end();

        // variables to store values in
        double _radius = 0; double _density = 0;
        double unused = 0;

        // parse the line
        qi::phrase_parse(begin, end,
                         double_ >> double_ >> double_ >> double_ >> double_ >> double_ >>
                         double_ >> double_ >> double_ >> double_,
                         char_(','),
                         _radius, unused, _density, unused, unused, unused, unused, unused,
                         unused, unused);

        // and save the value into the vectors
        density.push_back(_density);
        radii.push_back(_radius);

    }

    // since they are stored in decreasing order, we reverse the vectors
    std::reverse(density.begin(), density.end());
    std::reverse(radii.begin(), radii.end());

    // and close the file
    inprem.close();

    // and return the pair of vectors
    return std::make_pair(radii, density);

}

double Earth::getDensity(const double r) const {

    // since spacing is non-uniform, we have to search
    // for correct index
    // find the index above the desired value
    const int ru = static_cast<int>(std::lower_bound(this->data.first.begin(),
                                                     this->data.first.end(), r) - this->data.first.begin());
    // if we are at the edge
    if (ru == 0)
        return data.second[0];

    // clamp the lower index to be zero
    const int rl = ru - 1;

    // find values at surrounding indices
    const double r0 = this->data.first[static_cast<unsigned int>(rl)];
    const double r1 = this->data.first[static_cast<unsigned int>(ru)];
    const double d0 = this->data.second[static_cast<unsigned int>(rl)];
    const double d1 = this->data.second[static_cast<unsigned int>(ru)];

    // and then lerp into density space
    return d0 + (r - r0)*(d1 - d0)/(r1 - r0);
}
