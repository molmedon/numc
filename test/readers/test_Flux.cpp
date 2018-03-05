#include <string>
#include <iostream>
#include <readers/Flux.hpp>

#include <doctest.h>

using namespace anita::readers;

TEST_SUITE_BEGIN("flux");

TEST_CASE("Load ALL flux data files and evaluate first energy point using spline") {
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

TEST_SUITE_END();
