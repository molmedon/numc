#include <memory>

#include <Lepton.hpp>
#include <Neutrino.hpp>
#include <Particle.hpp>

using namespace anita;

/// Return the primary particle from a neutrino interaction
std::unique_ptr<Particle> TauNeutrino::getInteractionProducts(const Current current) const {

    // TODO; replace
    return std::make_unique<Tau>(18.);
}

// get the energy loss of the Tau Neutrino
double TauNeutrino::getEnergyLoss() const {

    // we provide the various 3-parameter parametrizations in
    // the appendix of arXiv::1704.00050
    // the following arrays are {beta_0, beta_1, beta_2}
    // Beta(E) = beta0 + b1*ln(E/E0) + b2*(ln(E/E0))^2
    // where E0 = 10^10 GeV and E is in GeV
    //
    // with E and E0 in log10(eV) space, this is
    // beta0 + beta1*ln(10)*(E - 19) beta2*(ln(10)*(E -19))^2

    // we create a lambda expression and wrap E in a closure
    // so that we only need to provide the coefficients
    // double E = this->getEnergy();
    auto Evaluate = [E = this->getEnergy()](std::array<double, 3> coeff) -> double {

        // convert base 10 energies to natural logarithm
        const double En = log(10)*(E - 19);

        // and return
        return coeff[0] + coeff[1]*En + coeff[2]*En*En;

    };

    // BDHM
    if (this->energy_loss_model == EnergyLossModel::BDHM) {
        std::array<double, 3> coeff = {0.425, 4.04e-2, 1.12e-3};
        return Evaluate(coeff);
    }
    // Soyez
    else if (this->energy_loss_model == EnergyLossModel::Soyez) {
        std::array<double, 3> coeff = {0.371, 3.20e-2, 9.54e-4};
        return Evaluate(coeff);
    }
    // Soyez-ASW
    else if (this->energy_loss_model == EnergyLossModel::Soyez_ASW) {
        std::array<double, 3> coeff = {0.461, 3.90e-2, 1.13e-3};
        return Evaluate(coeff);
    }
    // ALLM
    else if (this->energy_loss_model == EnergyLossModel::ALLM) {
        std::array<double, 3> coeff = {1.020, 0.210,   1.51e-2};
        return Evaluate(coeff);
    }
    // we have an unknown energy loss model
    else {
        std::cerr << "Unknown energy loss model. Quitting.." << std::endl;
        throw std::exception();
    }

}
