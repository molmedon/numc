#pragma once

#include <vector>

namespace anita { namespace readers {

        class FlightData {

        public:
            // Upon initialization, load flight data for ANITA4
            // or ANITA5 (specified by flight number)
            FlightData(int flight_number);
            ~FlightData();

        private:
            // these store the flight data parameters
            std::vector<float> heading;   // degrees
            std::vector<float> latitude;  // degrees
            std::vector<float> longitude; // degrees
            std::vector<float> altitude;  // km


        }; // END: class FlightData

    } // END: namespace readers
} // END: namespace anita
