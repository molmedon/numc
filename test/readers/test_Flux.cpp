#include <string>
#include <vector>
#include <iostream>
#include <root/TFile.h>
#include <root/TAxis.h>
#include <root/TGraph.h>
#include <root/TCanvas.h>
#include <readers/Flux.hpp>

#include <doctest.h>

using namespace anita::readers;

TEST_SUITE_BEGIN("flux");

///
/// \brief Load all available flux models and check that their initial value is as expected
///
TEST_CASE("LOAD FLUX MODELS") {
    CHECK(Flux(std::string("ahlers")).getFlux(16) == doctest::Approx(-13.98404));
    CHECK(Flux(std::string("allard")).getFlux(16) == doctest::Approx(-16.08282));
    CHECK(Flux(std::string("ave_min")).getFlux(16) == doctest::Approx(-18.46));
    CHECK(Flux(std::string("ave_max")).getFlux(16) == doctest::Approx(-17.4221));
    CHECK(Flux(std::string("Ave2005_Fe_Emax21.0")).getFlux(12.2555) == doctest::Approx(-17.0819));
    CHECK(Flux(std::string("Ave2005_Fe_Emax21.5")).getFlux(12.2555) == doctest::Approx(-17.0819));
    CHECK(Flux(std::string("Ave2005_Fe_Emax22.0")).getFlux(12.2675) == doctest::Approx(-17.0863));
    CHECK(Flux(std::string("Ave2005_Fe_hi_evo")).getFlux(11.9981) == doctest::Approx(-16.4456));
    CHECK(Flux(std::string("Ave2005_Fe_low_evo")).getFlux(12.0111) == doctest::Approx(-17.0869));
    CHECK(Flux(std::string("e-2")).getFlux(16) == doctest::Approx(-13.7));
    CHECK(Flux(std::string("essbaseline")).getFlux(16) == doctest::Approx(-16.86521));
    CHECK(Flux(std::string("essfig9")).getFlux(16) == doctest::Approx(-16.15268));
    CHECK(Flux(std::string("ess_n0")).getFlux(16) == doctest::Approx(-18.0495));
    CHECK(Flux(std::string("kkss_envo")).getFlux(16) == doctest::Approx(-14.86617));
    CHECK(Flux(std::string("gzk_peter")).getFlux(16) == doctest::Approx(-16.0));
    CHECK(Flux(std::string("Kotera2010_Fe_only")).getFlux(14.0054) == doctest::Approx(-16.8185));
    CHECK(Flux(std::string("Kotera2010_Fe_rich")).getFlux(14.0054) == doctest::Approx(-16.6006));
    CHECK(Flux(std::string("Kotera2010_mix_max")).getFlux(14) == doctest::Approx(-14.654));
    CHECK(Flux(std::string("Kotera2010_mix_min")).getFlux(14.0054) == doctest::Approx(-15.8687));
    CHECK(Flux(std::string("Stanev2008_heavy")).getFlux(14.0254) == doctest::Approx(-19.9959));
    CHECK(Flux(std::string("waxgzk")).getFlux(16) == doctest::Approx(-17.4275));
    CHECK(Flux(std::string("yuksel_grb")).getFlux(16) == doctest::Approx(-16.27533));
    CHECK(Flux(std::string("yuksel_qso")).getFlux(16) == doctest::Approx(-16.70348));
    CHECK(Flux(std::string("yuksel_sfh")).getFlux(16) == doctest::Approx(-16.94323));
}

///
/// \brief Produce a TFile containing TGraph's of all the available flux models. Saves to FluxModels.root
///
TEST_CASE("GRAPH FLUX MODELS") {

    // create a new TFile to save the plot too
    TFile* file = new TFile((std::string(OUTPUT_DIR)+std::string("/FluxModels.root")).c_str(),
                            "RECREATE", "Flux Models");

    // create a new canvas
    TCanvas* canvas = new TCanvas("FluxCanvas", "Flux Models", 600, 600);

    // build a vector of energies to sample at
    std::vector<double> energies;
    for (double E = 14; E <= 21; E += 0.5)
        energies.push_back(E);

    // build a vector of all flux models
    std::vector<std::string> flux_models = {"ahlers", "allard", "ave_min", "ave_max", "Ave2005_Fe_Emax21.0", "Ave2005_Fe_Emax21.5",
                                            "Ave2005_Fe_Emax22.0", "Ave2005_Fe_hi_evo", "Ave2005_Fe_low_evo", "e-2", "essbaseline",
                                            "essfig9", "ess_n0", "kkss_envo", "gzk_peter", "Kotera2010_Fe_only", "Kotera2010_Fe_rich",
                                            "Kotera2010_mix_max", "Kotera2010_mix_min", "Stanev2008_heavy", "waxgzk", "yuksel_grb",
                                            "yuksel_qso", "yuksel_sfh"};

    // and we loop over the name of the all the flux models
    for (auto& s: flux_models) {
        // load the flux model
        Flux flux(s);

        // loop over the flux models and evaluate the points
        std::vector<double> flux_vals;
        std::vector<double> energy_vals;
        for (auto& E: energies) {

            // limit the graphs to the range of the flux models
            if ((E < flux.min_energy) || (E > flux.max_energy))
                continue;
            else {
                energy_vals.push_back(E);
                flux_vals.push_back(flux.getFlux(E));
            }
        }

        // and now plot all the fluxes
        // this could be created before the loop and then call graph->DrawGraph(..., ..., ...)
        // but I couldn't get it to work - complained about no points
        TGraph* graph = new TGraph(static_cast<int>(energy_vals.size()), &energy_vals[0], &flux_vals[0]);

        // draw the graph
        graph->Draw("AC*");

        // set the title and name
        graph->SetTitle(s.c_str());
        graph->SetName(s.c_str());

        // set the axis labels
        graph->GetXaxis()->SetTitle("Energy [log_{10}(eV)]");
        graph->GetYaxis()->SetTitle("E dN/dE [eV/cm^{2} sr s]");

        // and save to a file
        graph->Write();

        // clear up memory
        delete graph;

    }

    // clear up the remaining memory
    delete canvas;
    delete file;

}

TEST_SUITE_END();
