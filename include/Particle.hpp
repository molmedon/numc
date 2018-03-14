#pragma once

#include <map>
#include <iostream>
#include <NuMC.hpp>
#include <functional>
#include <Continent.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

namespace anita {


    ///
    /// \brief Interaction type
    ///
    enum class InteractionType { Current, Decay };

    ///
    /// \brief Charged or Neutral Current enum
    ///
    enum class Current { Charged, Neutral };

    ///
    /// \brief Neutrino Flavor
    ///
    enum class Flavor { Electron, Muon, Tau };

    ///
    /// \brief Energy Loss Model
    ///
    enum class EnergyLossModel { BDHM, Soyez, Soyez_ASW, ALLM };

    ///
    /// \brief Abstract class representing a general particle - neutrino or lepton
    ///
    /// This abstract class contains the information shared between the neutrino
    /// and lepton base classes.
    /// Any method or function that can accept neutrinos or leptons
    /// should use Particle& or Particle* as the argument type, but if you wish to
    /// restrict behaviour to neutrinos or leptons, please use their
    /// respective subclasses.
    ///
    class Particle {

    public:

        ///
        /// \brief Get the energy of the particle in log10(eV) units
        ///
        double getEnergy() const { return this->energy; };

        ///
        /// \brief Set the energy of the particle in log10(eV) units
        ///
        void setEnergy(const double E) { this->energy = E; };

        ///
        /// \brief Flag indicating whether particle is neutrino
        ///
        bool isNeutrino() { return this->neutrino; };

        ///
        /// \brief Flag indicating whether particle is neutrino
        ///
        bool isLepton() { return !this->neutrino; };

        ///
        /// \brief Get the energy loss, dE/dX of the particle in GeV cm^2/g
        ///
        virtual double getEnergyLoss() const = 0;

        ///
        /// \brief Compute the interaction length at a density in g/cm^3 and return the interaction type
        ///
        virtual std::pair<double, InteractionType> getInteractionLength(const double density) const = 0;

        ///
        /// \brief Construct a new Particle with an energy given in log10(eV)
        ///
        Particle(const double E, const bool _neutrino) : energy(E), neutrino(_neutrino) {

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

        // virtual destructor since this class is abstract
        virtual ~Particle() {};


    private:

        ///
        /// \brief The energy of the particle in log10(eV) units
        ///
        double energy;

        ///
        /// \brief Whether this particle is a neutrino or lepton
        ///
        bool neutrino;

    };

} // END: namespace anita
