#pragma once

#include <string>
#include <readers/FlightData.hpp>

namespace anita {

    // This class represents a general version of ANITA
    // The subclasses, ANITA4 and ANITA5, are used to customize
    // the behaviour for each flight, but properties that are shared
    // across flights should be kept in the parent class
    class ANITA {

    public:

        // Initialize ANITA given a flight number. Currently 4 or 5.
        ANITA(int flight);
        ~ANITA() {};

    protected:

        // 4 or 5
        const int flight_number;

        // a class to store the flight data - latitude, longitude, altitude and heading
        const readers::FlightData flight_data;

    private:

    };


    // This class represents a complete ANITA-4 instrument
    class ANITA4 : public ANITA {

    public:
        ANITA4();
        ~ANITA4() {};

    private:

    };

    // This class represents a complete ANITA-5 instrument
    class ANITA5 : public ANITA {

    public:
        ANITA5();
        ~ANITA5() {};

    private:

    };


} // END: namespace anita
