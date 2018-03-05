#include <ANITA.hpp>

using namespace anita;

ANITA::ANITA(int flight) : flight_number(flight), flight_data(readers::FlightData(flight)) {


}
