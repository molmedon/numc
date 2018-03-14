#include <memory>
#include <iostream>
#include <Random.hpp>
#include <Particle.hpp>
#include <Neutrino.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

using namespace anita;

// default cross section model  for neutrino's
CrossSectionModel Neutrino::cross_section_model = CrossSectionModel::ConnollyMiddle;
EnergyLossModel Neutrino::energy_loss_model = EnergyLossModel::BDHM;

//  Compute the interaction length at a density in g/cm^3 and return the interaction type
std::pair<double, InteractionType> Neutrino::getInteractionLength(const double density) const {

    return std::make_pair(0, InteractionType::Current);
}



// use the Y-factor tables to return a Y-factor for the desired interaction
double Neutrino::getYFactor(const Current current) const {

    // we use the pre-loaded final state table to draw a random y-factor
    // as the data files have pre-sampled randomness in them
    if (current == Current::Charged) {
        this->chargedTable().evaluate(this->getEnergy());
    }
    else if (current == Current::Neutral) {
        this->neutralTable().evaluate(this->getEnergy());
    }
    else {
        std::cerr << "Unknown current in getYFactor" << std::endl;
        throw std::exception();
    }

    // to silence the compiler
    return -1;
}


// return the cross section for the desired interaction type
double Neutrino::getCrossSection(const Current current) const {

    if (current == Current::Neutral) {
        return getChargedCurrentCrossSection(this->getEnergy(), this->cross_section_model);
    }
    else if (current == Current::Charged) {
        return getNeutralCurrentCrossSection(this->getEnergy(), this->cross_section_model);
    }
    else {
        std::cerr << "Unknown current interaction. Quitting..." << std::endl;
        throw std::exception();
    }
}

// generate a random neutrino (e, mu, or t) with a given E
// in log10 eV units
std::unique_ptr<Neutrino> Neutrino::generateRandomNeutrino(const double energy) {

    // generate a random random associated
    Flavor randomFlavor = static_cast<Flavor>(uniformInt(0, 2)); // for three neutrino flavors

    // switch on the the random flavor
    switch (randomFlavor) {

        // we generate an electron neutrino
    case Flavor::Electron:
        return std::make_unique<ElectronNeutrino>(energy);

        // we generate a muon neutrino
    case Flavor::Muon:
        return std::make_unique<MuonNeutrino>(energy);

        // we generate a tau neutrino
    case Flavor::Tau:
        return std::make_unique<TauNeutrino>(energy);

        // something is wrong
    default:
        std::cerr << "Generating unknown Flavor in generateRandomNeutrino. Something is wrong..."
                  << std::endl;
        throw std::exception();
    }
}
