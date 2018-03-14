#pragma once

#include <array>
#include <Constants.hpp>

/// This pragma produces compile time warnings containing a certain message
/// Use like TODO(Check that this is correct)
#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))

namespace anita {

    ///
    /// \brief The number of frequency bands to simulate
    ///
    constexpr unsigned int NFREQ = 10;

    ///
    /// \brief The number of integration intervals per frequency band
    ///
    constexpr unsigned int FINTER_PER_BAND = 5;

    ///
    /// \brief The center frequency of each band in MHz
    ///
    const std::array<double, NFREQ> NUBAR = { 250., 350., 450., 550., 650., 750., 850., 950., 1050., 1150. };

    ///
    /// \brief The width of each frequency band in MHz
    ///
    const std::array<double, NFREQ> BWIDTH = { 100., 100., 100., 100., 100., 100., 100., 100., 100., 100. };


} // END: namespace anita
