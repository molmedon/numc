#include <iostream>
#include <Random.hpp>
#include <Particle.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

using namespace anita;

// check that particle energy is reasonable in our units
Particle::Particle(const double E) : energy(E) {

    // in log10(eV) units, a larger than necessary range
    // is 6-24 i.e. 1 MeV to YeV
    // this is here to catch stupid bugs involving units
    // propagator has separate min/max energy cuts for propagation
    if ((E <= 6) || (E >= 24)) {
        std::cerr << "Energy " << E
                  << " is not in the correct units of log10(eV)" << std::endl;
        throw std::exception();
    }
}

std::pair<double, Current> Particle::getInteractionLength(const double density) const {

    // get charged current interaction length
    double CClength = 1.0/(density*this->getCrossSection(Current::Charged));
    double NClength = 1.0/(density*this->getCrossSection(Current::Neutral));

    if (CClength > NClength) {
        return std::pair(NClength, Current::Neutral);
    }
    else {
        return std::pair(NClength, Current::Charged);
    }
}


double Particle::getYFactor(const Current current) const {

    return 0.2;
}

Particle Particle::getInteractionProducts(const Current current) const {

    return Particle(0);
}

double Particle::getCrossSection(const Current current) const {

    return 0;
}
