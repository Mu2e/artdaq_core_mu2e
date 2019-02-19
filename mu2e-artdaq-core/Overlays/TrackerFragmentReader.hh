#ifndef mu2e_artdaq_core_Overlays_TrackerFragmentReader_hh
#define mu2e_artdaq_core_Overlays_TrackerFragmentReader_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include "mu2e-artdaq-core/Overlays/DetectorFragment.hh"

#include <bitset>
#include <iostream>
#include <ostream>
#include <vector>

namespace mu2e {
class TrackerFragmentReader;
}

class mu2e::TrackerFragmentReader : public mu2e::DetectorFragment
{
public:
	TrackerFragmentReader(artdaq::Fragment const& f);
	~TrackerFragmentReader();

	// Tracker-specific methods:
	mu2e::DetectorFragment::adc_t strawIndex();
	mu2e::DetectorFragment::adc_t firstTDC();
	mu2e::DetectorFragment::adc_t secondTDC();
	std::vector<mu2e::DetectorFragment::adc_t> trackerADC();
	void printAll();
};

mu2e::TrackerFragmentReader::TrackerFragmentReader(artdaq::Fragment const& f)
	: DetectorFragment(f) {}

mu2e::TrackerFragmentReader::~TrackerFragmentReader() {}

mu2e::DetectorFragment::adc_t mu2e::TrackerFragmentReader::strawIndex()
{
	// The tracker data packets begin 8*16=128 bits after the beginning of the fragment data
	return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin() + 8), 127 - 8 * 2, 127 - 8 * 0);
}

mu2e::DetectorFragment::adc_t mu2e::TrackerFragmentReader::firstTDC()
{
	return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin() + 8), 127 - 8 * 4, 127 - 8 * 2);
}

mu2e::DetectorFragment::adc_t mu2e::TrackerFragmentReader::secondTDC()
{
	return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin() + 8), 127 - 8 * 6, 127 - 8 * 4);
}

std::vector<mu2e::DetectorFragment::adc_t> mu2e::TrackerFragmentReader::trackerADC()
{
	std::vector<adc_t> theVector;
	std::bitset<128> bitarray;
	fillBitArray(bitarray, dataBlockBegin() + 8);

	for (int i = 0; i < 8; i++) {
		theVector.push_back(convertFromBinary(bitarray, 127 - 8 * 6 - 10 * (1 + i), 127 - 8 * 6 - 10 * (0 + i)));
	}

	return theVector;
}

void mu2e::TrackerFragmentReader::printAll()
{
	std::cout << "\t\t"
			  << "Straw Index: " << (int)strawIndex() << std::endl;
	std::cout << "\t\t"
			  << "First TDC:   " << (int)firstTDC() << std::endl;
	std::cout << "\t\t"
			  << "Second TDC:  " << (int)secondTDC() << std::endl;
	std::cout << "\t\t"
			  << "ADC:         {[";
	std::vector<mu2e::DetectorFragment::adc_t> ADCarray = trackerADC();
	for (int i = 0; i < 8; i++) {
		std::cout << (int)ADCarray[i];
		if (i < 7) {
			std::cout << ",";
		}
	}
	std::cout << "]}" << std::endl;
}

#endif /* mu2e_artdaq_core_Overlays_TrackerFragmentReader_hh */
