#pragma once

#include <map>
#include <functional>

#include <NuMC.hpp>
#include <Continent.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

namespace anita {

    enum class Current { Charged, Neutral };

    enum class Flavor { Electron, Muon, Tau };

    // This class represents an abstract particle - both neutrino's
    // and leptons (as we track both through the Earth)
    // Any method or function that can accept neutrinos or leptons
    // should use Particle& as the argument type, but if you wish to
    // restrict behaviour to neutrinos or leptons, please use their
    // respective subclasses
    class Particle {

    public:

        // the energy of the particle in log10 eV units
        double energy;

        // get the cross section as a function of energy for NC or CC
        virtual double getCrossSection(const Current current) const;

        // get the Bjorken Y-factor at the current particle energy
        virtual double getYFactor(const Current current) const;

        // get the interaction length at a given density
        std::pair<double, Current> getInteractionLength(const double density) const;

        // get the primary particle resulting from an interaction of a certain kind
        virtual Particle getInteractionProducts(const Current current) const;

        // lepton
        bool isLepton;

        Particle(double E);
        ~Particle() {};


    private:

    };


}
