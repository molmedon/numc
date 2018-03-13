#include <doctest.h>
#include <root/TFile.h>
#include <root/TAxis.h>
#include <root/TGraph.h>
#include <root/TCanvas.h>
#include <readers/Earth.hpp>

TEST_SUITE("earth") {

    TEST_CASE("CREATE EARTH") {
        const anita::readers::Earth earth = anita::readers::Earth();
    }

    // test a few basic values hand picked from the PREM file
    TEST_CASE("BASIC TESTS") {

        // construct an Earth
        const anita::readers::Earth earth = anita::readers::Earth();

        // these values were picked somewhat uniformly throughout the PREM range
        CHECK(earth.getDensity(6371) == 1.02);
        CHECK(earth.getDensity(6370) == 1.02);
        CHECK(earth.getDensity(6369) == 1.02);
        CHECK(earth.getDensity(6363) == 2.6);
        CHECK(earth.getDensity(6354) == 2.9);
        CHECK(earth.getDensity(6345) == 3.38057);
        CHECK(earth.getDensity(6331) == 3.37905);
        CHECK(earth.getDensity(6251) == 3.37036);
        CHECK(earth.getDensity(6151) == 3.43577);
        CHECK(earth.getDensity(6061) == 3.48952);
        CHECK(earth.getDensity(5971) == 3.72375);
        CHECK(earth.getDensity(5871) == 3.84978);
        CHECK(earth.getDensity(5771) == 3.97582);
        CHECK(earth.getDensity(5685.7) == 4.39029);
        CHECK(earth.getDensity(5285.7) == 4.62956);
        CHECK(earth.getDensity(4785.7) == 4.90547);
        CHECK(earth.getDensity(4285.7) == 5.16395);
        CHECK(earth.getDensity(3785.7) == 5.41395);
        CHECK(earth.getDensity(3480) ==  9.90344);
        CHECK(earth.getDensity(2500) == 11.19067);
        CHECK(earth.getDensity(1600) == 11.94681);
        CHECK(earth.getDensity(900) == 12.91213);
        CHECK(earth.getDensity(200) == 13.07979);
        CHECK(earth.getDensity(100) == 13.08632);
        CHECK(earth.getDensity(0) == 13.08850);

    }

    // plot density as a function of depth
    TEST_CASE("PLOT DENSITY") {

        // construct an Earth
        const anita::readers::Earth earth = anita::readers::Earth();

        // we create a high density map between radius and density
        std::vector<double> radii;
        std::vector<double> density;
        for (double r = 0; r <= earth.max_radius; r += earth.max_radius/1000) {
            radii.push_back(r);
            density.push_back(earth.getDensity(r));
        }

        // and now we can plot this

        // create a new TFile to save the plot in
        TFile* file = new TFile((std::string(OUTPUT_DIR)+std::string("/EarthDensity.root")).c_str(),
                                "RECREATE", "Earth Density");

        // create a new canvas
        TCanvas* canvas = new TCanvas("FluxCanvas", "Flux Models", 600, 600);

        // and plot
        TGraph* graph = new TGraph(static_cast<int>(radii.size()), &radii[0], &density[0]);

        // and draw
        graph->Draw("AC");

        // set the title and the name
        graph->SetTitle("Earth Density");
        graph->SetTitle("Earth Density");

        // set the axis labels
        graph->GetXaxis()->SetTitle("Radius [km]");
        graph->GetYaxis()->SetTitle("Density [g/cm^{3}]");

        // and write to a file
        graph->Write();

        // free up memory
        delete graph;
        delete canvas;
        delete file;

    }

}
