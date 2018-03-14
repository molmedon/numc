#pragma once

#include <memory>
#include <Particle.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

namespace anita {

    ///
    /// \brief Abstract class representing a neutrino
    ///
    /// This abstract class is used to represent a neutrino of any flavor.
    /// For functions that can acceps any kind of neutrino, use Neutrino&
    /// or Neutrino* as the argument type
    ///
    class Neutrino : public Particle {

    public:
        ///
        /// \brief The flavor of the neutrino subclass
        ///
        Flavor flavor;

        ///
        /// Get the cross section, in cm^2 for a given current (NC/CC)
        ///
        double getCrossSection(const Current current) const;

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        virtual double getEnergyLoss() const = 0;

        ///
        /// \brief Sample and return a Bjorken y-factor at the current energy
        ///
        double getYFactor(const Current current) const;

        ///
        /// \brief Set the cross section model for ALL neutrinos
        ///
        static void setCrossSectionModel(const CrossSectionModel model) { cross_section_model = model; };

        ///
        /// \brief Generate a random neutrino flavor with a given energy in log10(eV) units
        ///
        /// This generates a pointer to a neutrino of a random flavor.
        ///
        static std::unique_ptr<Neutrino> generateRandomNeutrino(const double E);

        ///
        /// \brief Compute the interaction length at a density in g/cm^3 and return the interaction type
        ///
        std::pair<double, InteractionType> getInteractionLength(const double density) const override;

        ///
        /// \brief Return the primary particle from a neutrino interaction
        ///
        /// For a charged particle interaction, this returns a lepton with
        /// an appropriately sampled energy. For a neutral current interaction,
        /// this returns a neutrino with the appropriately sampled energy.
        ///
        virtual std::unique_ptr<Particle> getInteractionProducts(const Current current) const = 0;

        ///
        /// \brief Construct a Neutrino with a given energy (in log10(eV) units) and flavor
        ///
        Neutrino(double E, Flavor flv) : Particle(E, true), flavor(flv) {};

        ///
        /// \brief Construct a Neutrino with a given energy (in log10(eV) units) and flavor
        ///
        Neutrino(double E) : Particle(E, true), flavor(static_cast<Flavor>(uniformInt(0, 2))) {};

        // Virtual desctructor as this class is abstract
        virtual ~Neutrino() {};

    protected:

        // we load the CTEQ5 CC and NC data files for neutrino interactions
        // these utilize C++11 "initialize by first call" so are only loaded
        // on the first instantiation of a particle
        // access tables with particle->chargedTable(); // note the exta parenthesis

        // the cross section to use for this particle when sampling cross section
        static CrossSectionModel cross_section_model;

        // the model to use for energy loss calculations
        static EnergyLossModel energy_loss_model;

        // Charged current final state files
        static const readers::YTable chargedTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/final_cteq5_cc_nu.data"));
        };

        // neutral current final state files
        static const readers::YTable neutralTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/final_cteq5_nc_nu.data"));
        };

    };


    ///
    /// \brief A concrete class representing an Electron Neutrino
    ///
    class ElectronNeutrino : public Neutrino {

    public:
        ///
        /// \brief Construct an ElectronNeutrino with given energy `E` in log10(eV) units
        ///
        ElectronNeutrino(double E) : Neutrino(E, Flavor::Electron) {};
        ~ElectronNeutrino() {};

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        double getEnergyLoss() const override { return 0; };

        ///
        /// \brief Return the primary particle from a neutrino interaction
        ///
        /// For a charged particle interaction, this returns a lepton with
        /// an appropriately sampled energy. For a neutral current interaction,
        /// this returns a neutrino with the appropriately sampled energy.
        ///
        std::unique_ptr<Particle> getInteractionProducts(const Current current) const override;

    private:

    };


    ///
    /// \brief A concrete class representing a Muon Neutrino
    ///
    class MuonNeutrino : public Neutrino {

    public:
        ///
        /// \brief Construct a MuonNeutrino with given energy `E` in log10(eV) units
        ///
        MuonNeutrino(double E) : Neutrino(E, Flavor::Muon) {};
        ~MuonNeutrino() {};

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        double getEnergyLoss() const override { return 0; };

        ///
        /// \brief Return the primary particle from a neutrino interaction
        ///
        /// For a charged particle interaction, this returns a lepton with
        /// an appropriately sampled energy. For a neutral current interaction,
        /// this returns a neutrino with the appropriately sampled energy.
        ///
        std::unique_ptr<Particle> getInteractionProducts(const Current current) const override;

    private:

    };


    ///
    /// \brief A concrete class representing a Tau Neutrino
    ///
    class TauNeutrino : public Neutrino {

    public:
        ///
        /// \brief Construct a MuonNeutrino with given energy `E` in log10(eV) units
        ///
        TauNeutrino(double E) : Neutrino(E, Flavor::Tau) {};
        ~TauNeutrino() {};

        ///
        /// \brief Get the energy loss, dE/dX, in GeV cm^2/g
        ///
        double getEnergyLoss() const override;

        ///
        /// \brief Return the primary particle from a neutrino interaction
        ///
        /// For a charged particle interaction, this returns a lepton with
        /// an appropriately sampled energy. For a neutral current interaction,
        /// this returns a neutrino with the appropriately sampled energy.
        ///
        std::unique_ptr<Particle> getInteractionProducts(const Current current) const override;

    private:

    };


} // END: namespace anita
