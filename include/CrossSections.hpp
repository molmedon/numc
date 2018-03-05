#pragma once

namespace anita {

    // a choice of three Connolly et al. cross section models
    enum class CrossSectionModel { ConnollyLower, ConnollyMiddle, ConnollyUpper,
                                   ALLM, ASW, Sarkar, CKMT };

    // get the neutral current cross section (in units of XX) at the energy E using model 'model'
    // E is in log10 eV
    double getChargedCurrentCrossSection(const double E, const CrossSectionModel model);

    // get the charged current cross section (in units of XX) at the energy E using model 'model'
    // E is in log10 eV
    double getNeutralCurrentCrossSection(const double E, const CrossSectionModel model);

}
