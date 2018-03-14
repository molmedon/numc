#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include <NuMC.hpp>
#include <ANITA.hpp>
#include <Continent.hpp>
#include <Propagator.hpp>

using namespace anita;

int main(int argc, char** argv) {

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////// COMMAND LINE PARSING ////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // build command line parser
    namespace po = boost::program_options;

    // declare supported options
    po::options_description desc("Propagate neutrinos through the earth and record interaction vertices.");
    desc.add_options()
        ("help", "Print help messages")

        // general options
        ("num-events", po::value<int>()->required(), "Number of incident neutrinos")

        // options for particle propagation
        ("spectrum", po::value<std::string>()->required()->default_value("Kotera2010_mix_max"), "The neutrino spectrum file in data/fluxes/.")
        ("energy", po::value<double>()->default_value(0), "Incident energy of neutrinos in log10(eV) units if spectrum is 'fixed'.")
        ("min-energy", po::value<double>()->default_value(14.), "A minimum energy cut for propagation in log10(eV) units.")
        ("max-energy", po::value<double>()->default_value(20.9), "A maximum energy cut for propagation in log10(eV) units.")
        ("max-depth", po::value<double>()->default_value(50), "The maximum depth (in km) to save terminating hadronic air shower interactions.")
        ("nc-regeneration", po::value<bool>()->default_value(true), "Whether to use neutral current regeneration for neutrinos. If 'false', NC interactions terminate propagation.")

        // options for radio emission from particle interactions
        ("num-rays", po::value<int>()->default_value(100), "The number of rays to produce for every shower.")
        ("roughness", po::value<double>()->default_value(0), "The roughness scale of the Antarctic ice surface. Default value is 0 corresponding to smooth ice.")
        ("slope", po::value<bool>()->default_value(true), "Whether to use interpolated surface slope at each point.");

    // create variable map
    po::variables_map vm;
    try {
        // store command line options into variable map
        po::store(po::parse_command_line(argc, argv, desc), vm);

        // print the help description if the user didn't provide any arguments
        if (vm.count("help") || argc == 1) {
            std::cout << desc << std::endl;
            return true;
        }

        // throw exceptions if there are any problems (i.e. we didn't get required values)
        po::notify(vm);
    }

    // catch required option exception
    catch(po::required_option e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    // catch unknown option exception
    catch(po::unknown_option e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    //////////////////////////// START SIMULATION //////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // we create a representation of Antarctica
    // this create a new Continent() class that loads BEDMAP and other
    // data files relevant to particle propagation
    const Continent continent = Continent();

    // create a new propagator to propagate particles through the Earth using Kotera2010
    const Propagator propagator = Propagator(continent, vm["spectrum"].as<std::string>(), // flux model
                                             vm["energy"].as<double>(), // a fixed energy if desired, otherwise -1
                                             vm["min-energy"].as<double>(), // min energy cut
                                             vm["max-energy"].as<double>()); // max energy cut

    // we want to propagate 100 neutrinos through the Earth
    // this function is implicitly thread-safe
    propagator.propagateParticles(vm["num-events"].as<int>());

} // END: main
