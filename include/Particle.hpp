#pragma once

#include <map>
#include <functional>

#include <NuMC.hpp>
#include <Vector3.hpp>
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
        virtual double getCrossSection(const Current current) const = 0;

        // get the Bjorken Y-factor at the current particle energy
        virtual double getYFactor(const Current current) const = 0;

        // get the interaction length at a given density
        double getInteractionLength(const double density, const Current current) const;

        // lepton
        bool isLepton;

        Particle(double E);


    private:

    };

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

        Neutrino(double E, Flavor flv) : Particle(E), flavor(flv) {};

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


    // General lepton; use Lepton& for function arguments
    // that should only take leptons
    class Lepton : public Particle {

        // easy check if we are a lepton
        bool isLepton = true;

    public:
        Lepton(double E) : Particle(E) {};

    private:

    };

    // represents tau lepton and stores associated data files
    class TauLepton : public Lepton {

        // map from energy in log10(eV) units to crossSection in XXX
        // shared by all instances of TauLepton
        static std::map<double, double> crossSection;

    public:

        // tau decay products data table
        static const readers::YTable decayTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/tau_decay_tauola.data"));
        };

        TauLepton(double E) : Lepton(E) {};
    private:

    };


    // generate a random neutrino (e, mu, or t) with a given E
    // in log10 eV units
    Neutrino generateRandomNeutrino(double E);

    // generate a random neutrino energy according to a spectrum
    // or, if spectrum == EnergySpectrum::Fixed, then just return energy
    // double getNeutrinoEnergy(const EnergySpectrum spectrum, const double energy);

    // sample random energies from spectrum
    // double getNeutrinoEnergy(std::function<double (double)> flux);

    // get the Bjorken y-factor at a given energy
    // in units of log10 eV
    // double getYFactor(double energy);

}
