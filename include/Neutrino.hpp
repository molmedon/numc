#pragma once

#include <Particle.hpp>
#include <CrossSections.hpp>
#include <readers/Table.hpp>

namespace anita {

// virtual class to allow functions to only accept neutrinos as arguments
    class Neutrino : public Particle {
    public:
        // the flavor - quick and dirty way to check the subclass
        Flavor flavor;

        // we aren't a Tau lepton, Muon, Electron
        bool isLepton = false;

        // get the Bjorken Y-factor at the current particle energy
        double getYFactor(const Current current) const override;

        // the cross section to use for this particle when sampling cross section
        static CrossSectionModel cross_section_model;

        // change the cross section model for neutrinos
        void setCrossSectionModel(CrossSectionModel model) { cross_section_model = model; };

        // get the cross section as a function of energy
        double getCrossSection(const Current current) const override;

        // generate a random neutrino (e, mu, or t) with a given E
        // in log10 eV units
        static Neutrino generateRandomNeutrino(const double E);

        // get the decay products from a neutrino CC or NC interaction
        Particle getInteractionProducts(const Current current) const override;

        // construct a particular flavor
        Neutrino(double E, Flavor flv) : Particle(E), flavor(flv) {};

        // a constructor with only energy random creates a neutrino flavor
        Neutrino(double E) : Particle(E), flavor(static_cast<Flavor>(uniformInt(0, 2))) {};

    private:

        // we load the CTEQ5 CC and NC data files for neutrino interactions
        // these utilize C++11 "initialize by first call" so are only loaded
        // on the first instantiation of a particle
        // access tables with particle->chargedTable(); // note the exta parenthesis

        // Charged current final state files
        static const readers::YTable chargedTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/final_cteq5_cc_nu.data"));
        };

        // neutral current final state files
        static const readers::YTable neutralTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/final_cteq5_nc_nu.data"));
        };

    };

    class ElectronNeutrino : public Neutrino {

    public:
        ElectronNeutrino(double E) : Neutrino(E, Flavor::Electron) {};

    private:
    };

    class MuonNeutrino : public Neutrino {

    public:
        MuonNeutrino(double E) : Neutrino(E, Flavor::Muon) {};
    private:

    };

    class TauNeutrino : public Neutrino {

    public:
        TauNeutrino(double E) : Neutrino(E, Flavor::Tau) {};

    private:
    };


} // END: namespace anita
