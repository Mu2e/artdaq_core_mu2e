#ifndef mu2e_artdaq_core_Overlays_DebugFragmentReader_hh
#define mu2e_artdaq_core_Overlays_DebugFragmentReader_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include "mu2e-artdaq-core/Overlays/DetectorFragment.hh"

#include <bitset>
#include <iostream>
#include <ostream>
#include <vector>

namespace mu2e {
class DebugFragmentReader;
}

class mu2e::DebugFragmentReader : public mu2e::DetectorFragment {
 public:
  DebugFragmentReader(artdaq::Fragment const &f);
  ~DebugFragmentReader();

  // Debug Packet methods:
  mu2e::DetectorFragment::adc_t serialFIFO_empty();
  mu2e::DetectorFragment::adc_t serialFIFO_full();
  mu2e::DetectorFragment::adc_t numEmptyFIFO();
  mu2e::DetectorFragment::adc_t numFilledFIFO();
  std::vector<mu2e::DetectorFragment::adc_t> debugData();
  std::vector<std::vector<mu2e::DetectorFragment::adc_t> > debugInfo();
  void printAll();
};

mu2e::DebugFragmentReader::DebugFragmentReader(artdaq::Fragment const &f) : DetectorFragment(f) {}

mu2e::DebugFragmentReader::~DebugFragmentReader() {}

////////////////////////////////////////////////////////////////////
// Debug Header Packet methods
////////////////////////////////////////////////////////////////////
mu2e::DetectorFragment::adc_t mu2e::DebugFragmentReader::serialFIFO_empty() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()), 127 - 105, 127 - 104);
}

mu2e::DetectorFragment::adc_t mu2e::DebugFragmentReader::serialFIFO_full() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()), 127 - 106, 127 - 105);
}

mu2e::DetectorFragment::adc_t mu2e::DebugFragmentReader::numEmptyFIFO() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()), 127 - 120, 127 - 112);
}

mu2e::DetectorFragment::adc_t mu2e::DebugFragmentReader::numFilledFIFO() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()), 127 - 128, 127 - 120);
}
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Debug Data Packet methods
////////////////////////////////////////////////////////////////////

std::vector<mu2e::DetectorFragment::adc_t> mu2e::DebugFragmentReader::debugData() {
  std::vector<adc_t> theVector;
  // Start after at the 0th 16-bit block of the first debug data packet:
  for (size_t curPacketNum = 0; curPacketNum < packetCount(); curPacketNum++) {
    for (size_t adcIdx = 1; adcIdx < 8;
         adcIdx += 2) {  // The Deserialized Data is in every other position starting at 1
      theVector.push_back(*((adc_t const *)((dataBlockBegin() + 8 + 0) + adcIdx)));
    }
  }
  return theVector;
}

std::vector<std::vector<mu2e::DetectorFragment::adc_t> > mu2e::DebugFragmentReader::debugInfo() {
  std::vector<std::vector<adc_t> > theVector;
  // Start after at the 0th 16-bit block of the first debug data packet:
  for (size_t curPacketNum = 0; curPacketNum < packetCount(); curPacketNum++) {
    std::bitset<128> bitarray;
    fillBitArray(bitarray, dataBlockBegin() + 8 + curPacketNum * 8);
    for (size_t adcIdx = 0; adcIdx < 8; adcIdx += 2) {  // The info field is in every other position starting at 1
      std::vector<adc_t> infoVector;

      // FIFO Count
      infoVector.push_back(convertFromBinary(bitarray, 127 - adcIdx * 16 - 11, 127 - adcIdx * 16 - 0));

      // serialFifo_empty
      infoVector.push_back(convertFromBinary(bitarray, 127 - adcIdx * 16 - 13, 127 - adcIdx * 16 - 12));

      // serialFifo_full
      infoVector.push_back(convertFromBinary(bitarray, 127 - adcIdx * 16 - 14, 127 - adcIdx * 16 - 13));

      theVector.push_back(infoVector);
    }
  }
  return theVector;
}

// mu2e::DetectorFragment::adc_t mu2e::DebugFragmentReader::numSamples() {
//  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()+8),127-48,127-32);
//}

void mu2e::DebugFragmentReader::printAll() {
  std::cout << "\t\t"
            << "Binary Representation: ";
  printBitArray(bitArray(dataBlockBegin() + 8));

  std::cout << "\t\t"
            << "Packet Type:           " << (int)packetType() << std::endl;
  std::cout << "\t\t"
            << "Packet Count:          " << (int)packetCount() << std::endl;
  std::cout << "\t\t"
            << "Serial FIFO Empty:     " << (int)serialFIFO_empty() << std::endl;
  std::cout << "\t\t"
            << "Serial FIFO Full:      " << (int)serialFIFO_full() << std::endl;
  std::cout << "\t\t"
            << "# Empty FIFO Entries:  " << (int)numEmptyFIFO() << std::endl;
  std::cout << "\t\t"
            << "# Filled FIFO Entries: " << (int)numFilledFIFO() << std::endl;

  std::cout << "\t\t"
            << "Debug Data:         {[";
  std::vector<mu2e::DetectorFragment::adc_t> ADCarray = debugData();
  for (size_t i = 0; i < ADCarray.size(); i++) {
    std::cout << (int)ADCarray[i];
    if (i < ADCarray.size() - 1) {
      std::cout << ",";
    }
  }
  std::cout << "]}" << std::endl;

  std::cout << "\t\t"
            << "Debug Info:         {[";
  std::vector<std::vector<mu2e::DetectorFragment::adc_t> > InfoArray = debugInfo();
  for (size_t i = 0; i < InfoArray.size(); i++) {
    std::cout << "{" << (int)InfoArray[i][0] << "," << (int)InfoArray[i][1] << "," << (int)InfoArray[i][2] << "}";
    if (i < InfoArray.size() - 1) {
      std::cout << ",";
    }
  }
  std::cout << "]}" << std::endl;
}

#endif /* mu2e_artdaq_core_Overlays_DebugFragmentReader_hh */
