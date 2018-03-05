#include <iostream>
#include <Random.hpp>
#include <Particle.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

using namespace anita;

// default cross section model  for neutrino's
CrossSectionModel Neutrino::cross_section_model = CrossSectionModel::ConnollyMiddle;

// check that particle energy is reasonable in our units
Particle::Particle(double E) : energy(E) {

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

double Particle::getInteractionLength(const double density, const Current current) const {
    return 1.0/(density*this->getCrossSection(current));
}


double Neutrino::getYFactor(const Current current) const {

    // we use the pre-loaded final state table to draw a random y-factor
    // as the data files have pre-sampled randomness in them
    if (current == Current::Charged) {
        this->chargedTable().evaluate(this->energy);
    }
    else if (current == Current::Neutral) {
        this->neutralTable().evaluate(this->energy);
    }
    else {
        std::cerr << "Unknown current in getYFactor" << std::endl;
        throw std::exception();
    }

    // to silence the compiler
    return -1;
}



double Neutrino::getCrossSection(const Current current) const {

    if (current == Current::Neutral) {
        return getChargedCurrentCrossSection(this->energy, this->cross_section_model);
    }
    else if (current == Current::Charged) {
        return getNeutralCurrentCrossSection(this->energy, this->cross_section_model);
    }
    else {
        std::cerr << "Unknown current interaction. Quitting..." << std::endl;
        throw std::exception();
    }
}

// generate a random neutrino (e, mu, or t) with a given E
// in log10 eV units
Neutrino Neutrino::generateRandomNeutrino(const double energy) {

    // generate a random random associated
    Flavor randomFlavor = static_cast<Flavor>(uniformInt(0, 2)); // for three neutrino flavors

    // switch on the the random flavor
    switch (randomFlavor) {

        // we generate an electron neutrino
    case Flavor::Electron:
        return ElectronNeutrino(energy);

        // we generate a muon neutrino
    case Flavor::Muon:
        return MuonNeutrino(energy);

        // we generate a tau neutrino
    case Flavor::Tau:
        return TauNeutrino(energy);

        // something is wrong
    default:
        std::cerr << "Generated unknown Flavor in generateRandomNeutrino. Something is wrong..."
                  << std::endl;
        throw std::exception();
    }
}
