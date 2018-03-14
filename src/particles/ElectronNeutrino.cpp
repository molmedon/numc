#include <memory>

#include <Lepton.hpp>
#include <Neutrino.hpp>
#include <Particle.hpp>

using namespace anita;

/// Return the primary particle from a neutrino interaction
std::unique_ptr<Particle> ElectronNeutrino::getInteractionProducts(const Current current) const {

    // TODO; replace
    return std::make_unique<Electron>(18.);
}
