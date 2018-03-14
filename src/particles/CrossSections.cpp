#include <math.h>
#include <iostream>
#include <NuMC.hpp>
#include <CrossSections.hpp>

using namespace anita;

// get cross-section at a given energy for a given cross-section model
double anita::getChargedCurrentCrossSection(const double E, const CrossSectionModel model) {

    // the following cross section parametrizations are copeied directly from
    // https://github.com/harmscho/NuTauSim/
    // starting L626:simu_elost.cxx

    // we create a lambda expression and wrap E in a closure
    // so that we only need to provide the coefficients
    auto Evaluate = [E](double coeff[4]) -> double {
        // this stores the final cross section value
        double xsection = 0.;

        // iterate over the polynomial powers
        for (int i = 0 ; i < 4; i++){
            xsection += coeff[i]*pow(log10(E), i);
        }
        // and take a final power
        return pow(10, xsection);
    };

    switch (model) {

    case CrossSectionModel::ConnollyMiddle:
    {
        // Connolly et al. 2011 middle model (ARW's parametrization)
        double p[4] = {-5.35400180e+01, 2.65901551e+00, -1.14017685e-01, 1.82495442e-03};
        return Evaluate(p);
    }
    case CrossSectionModel::ConnollyLower:
    {
        // Connolly et al. 2011 lower model (ARW's parametrization)
        double p[4] = {-4.26355014e+01, 4.89151126e-01, 2.94975025e-02, -1.32969832e-03};
        return Evaluate(p);
    }

    case CrossSectionModel::ConnollyUpper:
    {
        // Connolly et al. 2011 upper model (ARW's parametrization){
        double p[4] = {-5.31078363e+01, 2.72995742e+00, -1.28808188e-01, 2.36800261e-03};
        return Evaluate(p);
    }
    case CrossSectionModel::ALLM:
    {
        // H. Abramowicz et al., Phys. Lett. B 269, 465 (1991);
        // H. Abramowicz and A. Levy, hep-ph/9712415.
        return pow(10, (-280.544665122 + 10.3452620208*pow(E, 0.317119535055))*1.e-36);
    }
    case CrossSectionModel::ASW:
    {
        // N. Armesto et al., Phys. Rev. D 77, 013001 (2008).
        // N. Armesto et al., Phys. Rev. Lett. 94, 022002 (2005).
        return pow(10, (-799.252409182+52.4932827684*pow(E,0.244551044541))*1.e-36);
    }
    case CrossSectionModel::Sarkar:
    {
        /* Sarkar */  // Default model used in Auger
        // A. Cooper-Sarkar and S. Sarkar, JHEP 0801, 075 (2008).
        // Amanda Cooper-Sarkar, Philipp Mertsch, Subir Sarkar. JHEP 08, 042 (2011).
        return pow(10, (-649.265343982+26.4437052803*pow(E,0.296160447336))*1.e-36);
    }
    case CrossSectionModel::CKMT:
    {
        /* CKMT */
        return pow(10, (-36.3345965603+7.14693605311*pow(E,0.293313250614))*1.e-36);
    }
    } // END: switch(model)

    std::cerr << "Unknown Charged Current CrossSectionModel. Using Middle model..." << std::endl;
    return anita::getChargedCurrentCrossSection(E, CrossSectionModel::ConnollyMiddle);
}


// get cross-section at a given energy for a given cross-section model
double anita::getNeutralCurrentCrossSection(const double E, const CrossSectionModel model) {

    // the following cross section parametrizations are copeied directly from
    // https://github.com/harmscho/NuTauSim/
    // starting L708:simu_elost.cxx

    // we create a lambda expression with wrap E in a closure
    // so that we only need to provide the coefficients
    auto Evaluate = [E](double coeff[4]) -> double {
        // this stores the final cross section value
        double xsection = 0.;

        // iterate over the polynomial powers
        for (int i = 0 ; i < 4; i++){
            xsection += coeff[i]*pow(log10(E), i);
        }
        // and take a final power
        return pow(10, xsection);
    };

    switch (model) {

    case CrossSectionModel::ConnollyMiddle:
    {
        // Connolly et al. 2011 middle model (ARW's parametrization)
        double p[4] = {-5.41463399e+01, 2.65465169e+00, -1.11848922e-01, 1.75469643e-03};
        return Evaluate(p);
    }
    case CrossSectionModel::ConnollyLower:
    {
        // Connolly et al. 2011 lower model (ARW's parametrization)
        double p[4] = {-4.42377028e+01, 7.07758518e-01, 1.55925146e-02, -1.02484763e-03};
        return Evaluate(p);
    }
    case CrossSectionModel::ConnollyUpper:
    {
        // Connolly et al. 2011 upper model (ARW's parametrization)
        double p[4] = {-5.36713302e+01,   2.72528813e+00,  -1.27067769e-01,   2.31235293e-03};
        return Evaluate(p);
    }
    // the below models have an additional factor of (1./2.4) according to
    // NuTauSim. What is the motivation for this? Need verification. RP
    case CrossSectionModel::ALLM:
    {
        // H. Abramowicz et al. Phys. Lett. B 269, 465 (1991);
        // H. Abramowicz and A. Levy, hep-ph/9712415.
        return pow(10, (1./2.4)*(-280.544665122 + 10.3452620208*pow(E, 0.317119535055))*1.e-36);
    }
    case CrossSectionModel::ASW:
    {
        // N. Armesto et al., Phys. Rev. D 77, 013001 (2008).
        // N. Armesto et al., Phys. Rev. Lett. 94, 022002 (2005).
        return pow(10, (1./2.4)*(-799.252409182+52.4932827684*pow(E,0.244551044541))*1.e-36);
    }
    case CrossSectionModel::Sarkar:
    {
        /* Sarkar */  // Default model used in Auger
        // A. Cooper-Sarkar and S. Sarkar, JHEP 0801, 075 (2008).
        // Amanda Cooper-Sarkar, Philipp Mertsch, Subir Sarkar. JHEP 08, 042 (2011).
        return pow(10, (-259.30822396 + 9.31732621406*pow(E, 0.302056103343))*1.e-36);
    }
    case CrossSectionModel::CKMT:
    {
        /* CKMT */
        return pow(10, (1./2.4)*(-36.3345965603+7.14693605311*pow(E,0.293313250614))*1.e-36);
    }
    } // END: switch

    std::cerr << "Unknown NeutralCurrent CrossSectionModel. Using Middle model..." << std::endl;
    return getNeutralCurrentCrossSection(E, CrossSectionModel::ConnollyMiddle);

}
