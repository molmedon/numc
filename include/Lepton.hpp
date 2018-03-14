#pragma once

#include <memory>

#include <Particle.hpp>

namespace anita {

    ///
    /// \brief An abstract class representing a general lepton.
    ///
    /// Classes that wish to take only leptons as arguments should use Lepton& or Lepton* as argument type.
    ///
    class Lepton : public Particle {

        ///
        /// \brief The flavor of the lepton - Electron, Muon, Tau
        ///
        Flavor flavor;

    public:
        ///
        /// \brief Construct a lepton of given energy `E` (in log10(eV) units) and flavor `flv`
        ///
        Lepton(double E, Flavor flv) : Particle(E, false), flavor(flv) {};

        ///
        /// \brief Compute the interaction length at a density in g/cm^3 and return the interaction type
        ///
        virtual std::pair<double, InteractionType> getInteractionLength(const double density) const override = 0;

        ///
        /// \brief Return the primary particle from a lepton interaction
        ///
        virtual std::unique_ptr<Particle> getInteractionProducts(const InteractionType interaction) const = 0;

    private:

    };


    ///
    /// \brief A concrete class representing an electron
    ///
    class Electron : public Lepton {

    public:

        ///
        /// \brief Construct an Electron with energy `E` in log10(eV) units
        ///
        Electron(double E) : Lepton(E, Flavor::Electron) {};

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        double getEnergyLoss() const override { return 0; };

        ///
        /// \brief Compute the interaction length at a density in g/cm^3 and return the interaction length/type
        ///
        std::pair<double, InteractionType> getInteractionLength(const double density) const override;

        ///
        /// \brief Return the primary particle from a lepton interaction
        ///
        std::unique_ptr<Particle> getInteractionProducts(const InteractionType interaction) const override;
    private:

    };


    ///
    /// \brief A concrete class representing a muon
    ///
    class Muon : public Lepton {

    public:

        ///
        /// \brief Construct a Muon with energy `E` in log10(eV) units
        ///
        Muon(double E) : Lepton(E, Flavor::Muon) {};

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        double getEnergyLoss() const override { return 0; };

        ///
        /// \brief Compute the interaction length at a density in g/cm^3 and return the interaction length/type
        ///
        std::pair<double, InteractionType> getInteractionLength(const double density) const override;

        ///
        /// \brief Return the primary particle from a lepton interaction
        ///
        std::unique_ptr<Particle> getInteractionProducts(const InteractionType interaction) const override;

    private:

    };


    ///
    /// \brief Construct a Tau with energy `E` in log10(eV) units
    ///
    class Tau : public Lepton {


    public:

        ///
        /// \brief Construct a Tau with energy `E` in log10(eV) units
        ///
        Tau(double E) : Lepton(E, Flavor::Tau) {};

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        double getEnergyLoss() const override { return 0; };

        ///
        /// \brief Compute the interaction length at a density in g/cm^3 and return the interaction length/type
        ///
        std::pair<double, InteractionType> getInteractionLength(const double density) const override;

        ///
        /// \brief Return the primary particle from a lepton interaction
        ///
        std::unique_ptr<Particle> getInteractionProducts(const InteractionType interaction) const override;

    private:

        // tau decay products data table
        static const readers::YTable decayTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/tau_decay_tauola.data"));
        };

    };

}
