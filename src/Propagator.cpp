#include <map>
#include <vector>
#include <NuMC.hpp>
#include <Random.hpp>
#include <Particle.hpp>
#include <Neutrino.hpp>
#include <Propagator.hpp>

#include <math.h>

using namespace anita;

// This function implements the complete propagation of a fixed number of particles
// drawn from a particular energy distribution
std::map<int, InteractionList> Propagator::propagateParticles(const int particlesToSimulate) const {


    // create a vector to store all valid interactions
    std::map<int, InteractionList> interactionMap;

    // iterate over the number of desired particles
    for (int n = 0; n < particlesToSimulate; n++) {

        // randomly pick an energy from the desired distribution
        // or a fixed energy if spectrum == energySpectrum::Fixed
        double energy = getRandomNeutrinoEnergy();

        // randomly select a neutrino flavor and create a new particle
        Neutrino neutrino = Neutrino::generateRandomNeutrino(energy);

        // // propagate the particle through the Earth and save it to the list
        interactionMap[n] = this->propagate(neutrino);

    }

    // and we are done
    return interactionMap;

}


InteractionList Propagator::propagate(Neutrino particle) const {
    // this function takes a Neutrino object and continually generates
    // random starting locations and directions and propagates the
    // particle through the Earth until the neutrino interacts
    // in such a way that it would generate a detectable pulse

    int ntrials = 1; // the number of particle attempts before a successful interaction
    while (ntrials) {

        // initialize a new vector to store interactions of this particle
        InteractionList interactions;

        // get random location on the surface of the sphere
        // TODO: VERIFY
        SphericalCoordinate surface = this->continent.getRandomSurfacePoint();

        // and a direction at the surface at this point
        // TODO: VERIFY
        SphericalCoordinate direction = this->continent.getRandomSurfaceDirection(surface);

        // and we compute the starting location of the particle on the surface of the Earth
        // TODO: VERIFY
        SphericalCoordinate start_point = SphericalCoordinate(PI - surface.theta,
                                                              surface.phi + PI,
                                                              surface.r); // geoid level

        // compute chord length of travel through Earth
        double chord_length = 2*this->continent.getSurfaceElevation(surface.theta, surface.phi)*cos(direction.theta);

        // we create a new Interaction to store the initial state of the propagation
        Interaction interaction = Interaction(0, particle, surface, direction,
                                              0, Current::Charged, chord_length, 0, 0);

        // we step through the Earth
        for (double distance = 0. ; distance < chord_length; )  {

            // we get the local density and material of the earth at the current location
            auto [density, material] = this->continent.getDensityAndMaterial(interaction.location);

            // get the interaction length at this density
            auto [interaction_length, current] = interaction.particle.getInteractionLength(density);

            // TODO: check for interaction and get current
            bool interacted = true; // replace this with whether we have interacted

            // we interact at this step
            if (interacted) {

                // get the particle resulting from this interaction or decay
                Particle new_particle = particle.getInteractionProducts(current);

                // TODO: change any relevant fields in the interaction object

                // and add the current interaction site to the list
                interactions.push_back(interaction);

            }
            else { // we did not interact at this step

            // update the interaction with the new position and info
            // interaction = Interaction(ntrials++, particle, location, direction,
            //                           depth, current, length, interaction.num_NC, interaction.num_CC);
            }

            // and increment the distance somehow?? whether we interact or not
            // distance +=


        } // END: while interaction.depth

        // if we get here, our particle did not make it to the surface
        ntrials++; // Peter has ntrials += 2. Why?

    } // END: while (ntrials)

}

double Propagator::getRandomNeutrinoEnergy() const {

    // if energy > 0, return the input energy - this lets us use a fixed energy
    if (this->fixed_energy > 0) {
        return this->fixed_energy;
    }

    // otherwise we use the Von Neumann method to sample from the flux distribution
    // we wrap flux->getFlux() in a lambda and a closure around "this"
    return sampleFromFunction([this](double E) { return this->flux.getFlux(E); },
                              this->min_energy, this->max_energy,
                              this->flux.min_flux, this->flux.max_flux);

}
