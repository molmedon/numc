#pragma once

#include <map>
#include <vector>
#include <NuMC.hpp>
#include <Particle.hpp>
#include <Neutrino.hpp>
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


    ///
    /// \brief A class to handle the propagation of source neutrinos through the Earth
    ///
    /// This class encapsulates all the necessary information for particle propagation, including a Continent
    /// representation, source neutrino flux model, and min/max energy cuts. Methods provided by this class
    /// allow for generating a random set of neutrino flavors and propagating them through the Earth, tracking
    /// a list of all the interactions that each neutrino undergoes during propagation.
    ///
    class Propagator {

    public:

        ///
        /// \brief Propagate a fixed number of particles through the Earth.
        ///
        /// propagateParticles takes as argument the number of neutrinos to simulate; it then loops, randomly generating
        /// neutrino flavors and energies from the source model. For each generated neutrino, it calls this->propagate()
        /// to simulate the interactions of that neutrino. The keys of each map correspond to each source neutrino
        ///
        /// @param particlesToSimulate The number of neutrinos to propagate through the Earth.
        ///
        std::map<int, InteractionList> propagateParticles(const int particlesToSimulate) const;

        /// \brief Propagate a single particle through the Earth recording all interactions.
        ///
        /// For each input neutrino, we pick a random exit location and random exit direction and back-calculate
        /// the source location and direction. The source neutrino is then propagated through the Earth, returning
        /// a vector of all the interactions it undergoes (NC and CC) during its propagation.
        ///
        /// @param particle The Neutrino to propagate through the Earth.
        ///
        InteractionList propagate(Neutrino particle) const;

        ///
        /// \brief Construct a new propagator.
        ///
        /// Construct a new propagator, loading all necessary data tables and cross sections, but uses
        /// a pre-constructed Continent object.
        ///
        /// @param con An initialized Continent object
        /// @param fluxname A string corresponding to a filename in data/flux which contains the desired source neutrino model
        /// @param fixedE If `fluxname`=='fixed', then `fixedE` will contain the energy used for all neutrinos (in log10(eV) units)
        /// @param maxE The maximum energy above which particles are not generated
        /// @param minE The minimum energy below which particles are not generated, or propagated
        ///
        Propagator(const Continent& con, const std::string fluxname, const double fixedE,
                   const double maxE, const double minE) : continent(con), flux_model(fluxname),
                                                           flux(readers::Flux(fluxname)), fixed_energy(fixedE),
                                                           min_energy(minE), max_energy(maxE) {};
    private:

        ///
        /// \brief Generates a random neutrino energy from the source flux model of the Propagator.
        ///
        double getRandomNeutrinoEnergy() const;

        ///
        /// \brief An initialized Continent object to provide access to Earth information.
        ///
        const Continent& continent;

        ///
        /// \brief the desired source neutrino spectrum/flux. This must correspond to a filename in data/flux.
        ///
        const std::string flux_model;

        ///
        /// \brief A reader for incident neutrino flux data files.
        ///
        const readers::Flux flux;

        ///
        /// \brief The fixed energy to use for all neutrinos if `flux_model == fixed`.
        ///
        const double fixed_energy;

        ///
        /// \brief The minimum energy below which neutrinos are not propagated.
        ///
        const double min_energy;

        ///
        /// \brief The maximum energy of source neutrino generation or propagation.
        ///
        const double max_energy;

    };


}
