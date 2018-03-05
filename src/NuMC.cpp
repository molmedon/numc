#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include <NuMC.hpp>
#include <ANITA.hpp>
#include <Continent.hpp>
#include <Propagator.hpp>

using namespace anita;

int main(int argc, char** argv) {

    // build command line parser
    namespace po = boost::program_options;

    // declare supported options
    po::options_description desc("Propagate neutrinos through the earth and record interaction vertices.");
    desc.add_options()
        ("help", "Print help messages")
        ("num-events", po::value<int>()->required(), "Number of incident neutrinos")
        ("spectrum", po::value<std::string>()->required()->default_value("Kotera2010_mix_max"), "The neutrino spectrum file")
        ("energy", po::value<double>()->default_value(-1.), "Incident energy of neutrinos in log10(eV) units")
        ("min-energy", po::value<double>()->default_value(14.), "A minimum energy cut for propagation in log10(eV) units")
        ("max-energy", po::value<double>()->default_value(20.9), "A maximum energy cut for propagation in log10(eV) units")
        ("maxdepth", po::value<double>()->default_value(10.), "The depth above which interactions will be saved and used for shower generation (km)")
        ("crossSectionFactor", po::value<double>()->default_value(1), "Factor to multiply by all cross sections");

    // create variable map
    po::variables_map vm;
    try {
        // store command line variables
        po::store(po::parse_command_line(argc, argv, desc), vm);

        // print the help descriptions
        if (vm.count("help") || argc == 1) {
            std::cout << desc << std::endl;
            return true;
        }

        // throw exceptions if there are any problems
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

    // we create a representation of Antarctica
    // this create a new Continent() class that loads BEDMAP and other
    // data files relevant to particle propagationx
    const Continent continent = Continent();

    // create a new propagator to propagate particles through the Earth using Kotera2010
    const Propagator propagator = Propagator(continent, std::string("Kotera2010_mix_max"), // flux model
                                             vm["energy"].as<double>(), // a fixed energy if desired, otherwise -1
                                             vm["min-energy"].as<double>(), // min energy cut
                                             vm["max-energy"].as<double>()); // max energy cut

    // we want to propagate 100 neutrinos through the Earth
    // this function is implicitly thread-safe
    propagator.propagateParticles(vm["num-events"].as<int>());

} // END: main
