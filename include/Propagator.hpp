#pragma once

#include <vector>
#include <NuMC.hpp>
#include <Particle.hpp>
#include <Continent.hpp>
#include <readers/Table.hpp>
#include <readers/Flux.hpp>

namespace anita {

    // This class represents a VALID particle interaction that satisfies
    // the following conditions:
    //     1) Occurs within the specified 'maxdepth' below the surface of the ice
    //     MORE?
    struct Interaction {

        // number of trials before a successful interaction
        int trials;

        // the particle in question - could be neutrino or tau lepton
        // this contains particle energy etc.
        Particle& particle;

        // theta, phi, r of particle interaction location w.r.t center of Earth
        SphericalCoordinate& location;

        // theta, phi of particle velocity at interaction point (r = 1)
        SphericalCoordinate& direction;

        // depth (in km) of interaction
        double depth;

        // interaction type - Current::Charged or Current::Neutral
        Current current;

        // total chord length of propagation before interaction
        double chord_length;

        // number of NC interactions before this interaction
        int num_NC;

        // number of CC interactions before this interaction
        int num_CC;

        // basic constructor - in order
        Interaction(int n, Particle& p, SphericalCoordinate& loc, SphericalCoordinate& vec,
                    double d, Current c, double L, int nc, int cc) : trials(n), particle(p), location(loc),
                                                                     direction(vec), depth(d), current(c),
                                                                     chord_length(L), num_NC(nc), num_CC(cc) {};
    };

    // a vector of operations - fundamental unit of simulation
    using InteractionList = typename std::vector<Interaction>;

    // This class encapsulates all the necessary information to propagate particles
    // through the Earth and produce a vector of interaction locations
    class Propagator {

    public:
        // A continent shared across all instances
        const Continent& continent;

        // the desired source neutrino spectrum/flux - this is a filename in the flux/ directory
        const std::string flux_model;

        // load the flux data file
        const readers::Flux flux;

        // a fixed energy of source neutrino energy, otherwise -1
        const double fixed_energy;

        // the minimum/maximum energy of propagation in log10 eV units
        const double min_energy;
        const double max_energy;

        // generate a random energy from the given neutrino flux_model
        double getRandomNeutrinoEnergy() const;

        // To ensure thread-safety, propagate() must be completely pure (in the functional sense)
        // Called via Propagator->propagate(100) during thread creation
        InteractionList propagateParticles(const int particlesToSimulate) const;

        // This propagates a single particle through the Earth
        Interaction propagate(Neutrino particle) const;

        // construct a new propagator, loading all necessary data tables and cross sections
        // using a pre-constructed continent
        Propagator(const Continent& con, const std::string fluxname, const double fixedE,
                   const double maxE, const double minE) : continent(con), flux_model(fluxname),
                                                           flux(readers::Flux(fluxname)), fixed_energy(fixedE),
                                                           min_energy(minE), max_energy(maxE) {};


    private:

    };


}
