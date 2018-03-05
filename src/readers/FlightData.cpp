#include <iostream>
#include <readers/FlightData.hpp>

#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>


using namespace anita::readers;

FlightData::FlightData(int flight_number) {

    // check that the flight number is only 4 or 5
    if ((flight_number != 4) and (flight_number != 5)) {
        std::cerr << "nuMC currently only supports ANITA-IV and ANITA-V";
        std::cerr << "Please specify a flight_number of 4 or 5" << std::endl;
        throw std::exception();
    }

    // construct the filename for this flight
    std::string filename = std::string(DATA_DIR);
    filename += "/anita" + std::to_string(flight_number) + "gps_pitchroll.root";

    // open the data file corresponding to this flight
    TFile *file = TFile::Open(filename.c_str());

    // check that we opened successfully
    if (!file || file->IsZombie()) {
        std::cerr << "Unable to open flight data file {" << filename << "). Quitting...";
        std::cerr << std::endl;
        throw std::exception();
    }

    // and the standard tree name for our flight data
    TTreeReader reader("adu5PatTree;1", file);

    // we define a lambda function to check the results of
    // reading arrays from the TTree
    auto CheckArray = [](TTreeReaderArray<float>& value) -> bool {
        if (value.GetSetupStatus() < 0) {
            std::cerr << "Error " << value.GetSetupStatus()
            << " setting up reader for " << value.GetBranchName() << " flight data. " << std::endl;
            throw std::exception();
        }
        return true;
    };

    // we construct TTreeReaders for all of the required values
    // and read them into our arrays
    TTreeReaderArray<float> headingArray(reader, "heading");
    CheckArray(headingArray); // this checks that nothing went wrong

    TTreeReaderArray<float> latitudeArray(reader, "latitude");
    CheckArray(latitudeArray); // this checks that nothing went wrong

    TTreeReaderArray<float> longitudeArray(reader, "longitude");
    CheckArray(longitudeArray); // this checks that nothing went wrong

    TTreeReaderArray<float> altitudeArray(reader, "altitude");
    CheckArray(altitudeArray); // this checks that nothing went wrong

    // we allocate memory for each of our flight data vectors
    this->heading.reserve(headingArray.GetSize());
    this->latitude.reserve(latitudeArray.GetSize());
    this->longitude.reserve(longitudeArray.GetSize());
    this->altitude.reserve(altitudeArray.GetSize());

    // we then loop over the readers saving the values into the arrays
    while (reader.Next()) {
        // use *mass, *charge, ...
    }
}

FlightData::~FlightData() {
}
