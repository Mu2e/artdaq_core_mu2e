#include "mu2e-artdaq-core/Overlays/DetectorFragment.hh"

#if 0
namespace {
  unsigned int pop_count (unsigned int n) {
    unsigned int c; 
    for (c = 0; n; c++) n &= n - 1; 
    return c;
  }
}
#endif

void mu2e::DetectorFragment::checkADCData(int daq_adc_bits) const {
  mu2e::DetectorFragment::adc_t const * adcPtr(findBadADC(daq_adc_bits));
  if (adcPtr != dataEnd()) {
    throw cet::exception("IllegalADCVal")
        << "Illegal value of ADC word #"
        << (adcPtr - dataBegin())
        << ": 0x"
        << std::hex
        << *adcPtr
        << ".";
  }
}

std::ostream & mu2e::operator << (std::ostream & os, DetectorFragment const & f) {
  os << "DetectorFragment event size: "
     << f.hdr_event_size()
     << ", run number: "
     << f.hdr_run_number()
     << "\n";

  return os;
}

//std::bitset<128> mu2e::DetectorFragment::bitArray(adc_t const * beginning) {
std::bitset<128> mu2e::DetectorFragment::bitArray(mu2e::DetectorFragment::adc_t const *beginning) const {
  // Return 128 bit bitset filled with bits starting at the indicated position in the fragment
  std::bitset<128> theArray;
  for(int bitIdx=127, adcIdx = 0; adcIdx<8; adcIdx++) {
    for(int offset = 0; offset<16; offset++) {
      if( ( (*((adc_t const *)(beginning+adcIdx))) & (1<<offset) ) != 0) {
	theArray.set(bitIdx);
      } else {
	theArray.reset(bitIdx);
      }
      bitIdx--;
    }
  }
  return theArray;
}

void mu2e::DetectorFragment::fillBitArray(std::bitset<128> &theArray, adc_t const * beginning) {
  // Fill bitset using the 128 bits starting at the indicated position in the fragment
  for(int bitIdx=127, adcIdx = 0; adcIdx<8; adcIdx++) {
    for(int offset = 0; offset<16; offset++) {
      if( ( (*((adc_t const *)(beginning+adcIdx))) & (1<<offset) ) != 0) {
	theArray.set(bitIdx);
      } else {
	theArray.reset(bitIdx);
      }
      bitIdx--;
    }
  }
}

void mu2e::DetectorFragment::printBitArray(std::bitset<128> theArray) {
  // Print all 128 bits in the packet
  std::cout << "\t\t" << "Packet Bits (128): ";
  for(int i=0; i<128; i++) {
    std::cout << theArray[i];
  }
  std::cout << std::endl;
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::convertFromBinary(std::bitset<128> theArray, int minIdx, int maxIdx) const {
  std::bitset<16> retVal;
  for(int i=minIdx+1; i<=maxIdx; i++) {
    retVal.set(maxIdx-i,theArray[i]);
  }
  return retVal.to_ulong();
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::byteCount() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()),127-16,127-0);
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::rocID() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()),127-20,127-16);
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::packetType() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()),127-24,127-20);
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::valid() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()),127-32,127-31);
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::packetCount() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()),127-43,127-32);
}

mu2e::DetectorFragment::adc_t mu2e::DetectorFragment::status() {
  return mu2e::DetectorFragment::convertFromBinary(bitArray(dataBlockBegin()),127-104,127-96);
}

std::vector<mu2e::DetectorFragment::adc_t> mu2e::DetectorFragment::timestampVector() {

  std::vector<adc_t> theVector;
  std::bitset<128> bitarray;
  fillBitArray(bitarray,dataBlockBegin());

  for(int i=0; i<6; i++) {
    theVector.push_back(convertFromBinary(bitarray,127-8*(6+i + 1),127-8*(6+i)));
  }

  return theVector;
}

std::vector<mu2e::DetectorFragment::adc_t> mu2e::DetectorFragment::dataVector() {

  std::vector<adc_t> theVector;
  std::bitset<128> bitarray;
  fillBitArray(bitarray,dataBlockBegin());

  for(int i=0; i<3; i++) {
    theVector.push_back(convertFromBinary(bitarray,127-8*(13+i + 1),127-8*(13+i)));
  }

  return theVector;
}

void mu2e::DetectorFragment::printDTCHeader() {
  //  std::cout << "\t\t" << "Binary Representation: ";
  //  printBitArray(bitArray(dataBlockBegin()));
  std::cout << "\t\t" << "ROC ID:         " << (int)rocID()       << std::endl;
  std::cout << "\t\t" << "Byte Count:     " << (int)byteCount()   << std::endl;
  std::cout << "\t\t" << "Packet Type:    " << (int)packetType()  << std::endl;
  std::cout << "\t\t" << "Valid:          " << (int)valid()       << std::endl;
  std::cout << "\t\t" << "Packet Count:   " << (int)packetCount() << std::endl;
  std::cout << "\t\t" << "Status:         " << (int)status()      << std::endl;
  std::cout << "\t\t" << "Timestamp:      {[";

  std::vector<mu2e::DetectorFragment::adc_t> tsVec = timestampVector();
  for(size_t i=0; i<tsVec.size(); i++) {
    std::cout << (int)tsVec[i];
    if(i<tsVec.size()-1) {
      std::cout << ",";
    }
  }
  std::cout << "]}" << std::endl;

  std::cout << "\t\t" << "Data:           {[";
  std::vector<mu2e::DetectorFragment::adc_t> dVec = dataVector();
  for(size_t i=0; i<dVec.size(); i++) {
    std::cout << (int)dVec[i];
    if(i<dVec.size()-1) {
      std::cout << ",";
    }
  }
  std::cout << "]}" << std::endl;

}


void mu2e::DetectorFragment::printAll() {
  printDTCHeader();
}
