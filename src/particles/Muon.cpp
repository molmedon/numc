#include <memory>

#include <Lepton.hpp>
#include <Neutrino.hpp>
#include <Particle.hpp>

using namespace anita;

// Return the interaction length and type
std::pair<double, InteractionType> Muon::getInteractionLength(const double density) const {

    return std::make_pair(0, InteractionType::Decay);
}

/// Return the primary particle from a neutrino interaction
std::unique_ptr<Particle> Muon::getInteractionProducts(const InteractionType interaction) const {

    // TODO; replace
    return std::make_unique<MuonNeutrino>(18.);
}
