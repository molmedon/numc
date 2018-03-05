#include <doctest.h>
#include <Particle.hpp>

TEST_SUITE_BEGIN("particle");

TEST_CASE("Creating a base neutrino") {

    SUBCASE("Electron Neutrino") {
        anita::Neutrino neutrino = anita::Neutrino(18., anita::Flavor::Electron);
    }

    SUBCASE("Muon Neutrino") {
        anita::Neutrino neutrino = anita::Neutrino(18., anita::Flavor::Muon);
    }

    SUBCASE("Tau Neutrino") {
        anita::Neutrino neutrino = anita::Neutrino(18., anita::Flavor::Tau);
    }
}

TEST_CASE("Creating an electron neutrino") {
    anita::ElectronNeutrino neutrino = anita::ElectronNeutrino(18.);
}

TEST_CASE("Creating an muon neutrino") {
    anita::MuonNeutrino neutrino = anita::MuonNeutrino(18.);
}

TEST_CASE("Creating an tau neutrino") {
    anita::TauNeutrino neutrino = anita::TauNeutrino(18.);
}


TEST_SUITE_END();
