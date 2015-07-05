#ifndef mu2e_artdaq_core_Overlays_TrackerFragmentWriter_hh
#define mu2e_artdaq_core_Overlays_TrackerFragmentWriter_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib/exception.h"

#include "mu2e-artdaq-core/Overlays/DetectorFragmentWriter.hh"

#include <iostream>
#include <ostream>
#include <vector>
#include <bitset>

namespace mu2e {
  class TrackerFragmentWriter;
}

class mu2e::TrackerFragmentWriter: public mu2e::DetectorFragmentWriter {

  public:

  TrackerFragmentWriter(artdaq::Fragment & f); 
  ~TrackerFragmentWriter(); 

  // Tracker-specific methods:
  mu2e::DetectorFragment::adc_t strawIndex();
  mu2e::DetectorFragment::adc_t firstTDC();
  mu2e::DetectorFragment::adc_t secondTDC();
  std::vector<mu2e::DetectorFragment::adc_t> trackerADC();
  void printAll();

};

mu2e::TrackerFragmentWriter::TrackerFragmentWriter(artdaq::Fragment& f ) :
  DetectorFragmentWriter(f) {
}

mu2e::TrackerFragmentWriter::~TrackerFragmentWriter() {
}

mu2e::DetectorFragment::adc_t mu2e::TrackerFragmentWriter::strawIndex() {
  // The tracker data packets begin 8*16=128 bits after the beginning of the fragment data
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()+8),127-8*2,127-8*0);
}

mu2e::DetectorFragment::adc_t mu2e::TrackerFragmentWriter::firstTDC() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()+8),127-8*4,127-8*2);
}

mu2e::DetectorFragment::adc_t mu2e::TrackerFragmentWriter::secondTDC() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()+8),127-8*6,127-8*4);
}

std::vector<mu2e::DetectorFragment::adc_t> mu2e::TrackerFragmentWriter::trackerADC() {

  std::vector<adc_t> theVector;
  std::bitset<128> bitarray;
  fillBitArray(bitarray,dataBlockBegin()+8);

  for(int i=0; i<8; i++) {
    theVector.push_back(convertFromBinary(bitarray,127-8*6-10*(1+i),127-8*6-10*(0+i)));
  }

  return theVector;
}

void mu2e::TrackerFragmentWriter::printAll() {
  std::cout << "\t\t" << "Straw Index: " << (int)strawIndex() << std::endl;
  std::cout << "\t\t" << "First TDC:   " << (int)firstTDC()   << std::endl;
  std::cout << "\t\t" << "Second TDC:  " << (int)secondTDC()  << std::endl;
  std::cout << "\t\t" << "ADC:         {[";
  std::vector<mu2e::DetectorFragment::adc_t> ADCarray = trackerADC();
  for(int i=0; i<8; i++) {
    std::cout << (int)ADCarray[i];
    if(i<7) {
      std::cout << ",";
    }
  }
  std::cout << "]}" << std::endl;
}

#endif /* mu2e_artdaq_core_Overlays_TrackerFragmentWriter_hh */
