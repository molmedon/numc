#pragma once

namespace anita {


    // General lepton; use Lepton& for function arguments
    // that should only take leptons
    class Lepton : public Particle {

        // easy check if we are a lepton
        bool isLepton = true;

    public:
        Lepton(double E) : Particle(E) {};

        // get the primary particle resulting from an interaction of a certain kind
        Particle getInteractionProducts(const Current current) const;

    private:

    };

    // represents an electron and stores associated data files
    class Electron : public Lepton {

        // map from energy in log10(eV) units to crossSection in XXX
        // shared by all instances of Electron
        static std::map<double, double> crossSection;

    public:

        Electron(double E) : Lepton(E) {};
    private:

    };

    // represents a muon and stores associated data files
    class Muon : public Lepton {

        // map from energy in log10(eV) units to crossSection in XXX
        // shared by all instances of Muon
        static std::map<double, double> crossSection;

    public:

        Muon(double E) : Lepton(E) {};
    private:

    };

    // represents tau lepton and stores associated data files
    class Tau : public Lepton {

        // map from energy in log10(eV) units to crossSection in XXX
        // shared by all instances of Tau
        static std::map<double, double> crossSection;

    public:

        // tau decay products data table
        static const readers::YTable decayTable() {
            return readers::YTable(std::string(DATA_DIR)+std::string("/tau_decay_tauola.data"));
        };

        Tau(double E) : Lepton(E) {};
    private:

    };

}
