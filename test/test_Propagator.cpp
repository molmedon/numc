#include <Continent.hpp>
#include <Propagator.hpp>

#include <doctest.h>

TEST_CASE("Propagator Initialization") {

    // construct a new continent
    const anita::Continent continent = anita::Continent();

    // create a new propagator
    const anita::Propagator _propagator = anita::Propagator(continent, std::string("Kotera2010_mix_max"),
                                                            -1, 14., 20.);
}

TEST_CASE("Propagator propagating particles") {

    // construct a new continent
    const anita::Continent continent = anita::Continent();

    // create a new propagator
    const anita::Propagator propagator = anita::Propagator(continent, std::string("Kotera2010_mix_max"),
                                                           -1., 14., 20.);

    SUBCASE("Propagate a single particle...") {

        // propagate one particle
        // Neutrino neutrino = getRandomNeutrino(
        // propagator.propagate(
    }

    SUBCASE("Propagate multiple particles...") {

        // propagate 10 particles through the Earth
        propagator.propagateParticles(10);
    }
}
