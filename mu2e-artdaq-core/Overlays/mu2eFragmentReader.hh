#ifndef mu2e_artdaq_core_Overlays_mu2eFragmentReader_hh
#define mu2e_artdaq_core_Overlays_mu2eFragmentReader_hh

////////////////////////////////////////////////////////////////////////
// mu2eFragmentReader
//
// Class derived from mu2eFragment which provides additional methods
// for accessing data stored in a mu2eFragment
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "artdaq-core/Data/Fragments.hh"
#include "mu2e-artdaq-core/Overlays/mu2eFragment.hh"

#include <bitset>

#include <iostream>
#include "trace.h"

namespace mu2e {
  class mu2eFragmentReader;
}

class mu2e::mu2eFragmentReader: public mu2e::mu2eFragment {
  public:

  typedef uint16_t adc_t;

  mu2eFragmentReader(artdaq::Fragment const & f) : mu2eFragment(f) {};

  size_t blockIndexBytes(size_t offset) const;

  size_t blockSizeBytes() const;
  size_t blockSizeBytes(size_t offset) const;
  size_t blockEndBytes(size_t offset) const;
  Header::data_t const * dataAtBytes(size_t offset) const;
  Header::data_t const * dataAtBlockIndex(size_t offset) const;

  void printPacketAtByte(size_t offset) const;

  protected:

  // Bit processing helper methods
  // For now, the focus is on clarity and ease of use, but eventually
  // these sorts of functions will need to be optimized for efficiency

  // bitArray populates a 128-bit bitset using the 128 bits beginning
  // at the position indicated by the provided pointer
  //  std::bitset<128> bitArray(adc_t const * beginning);
  std::bitset<128> bitArray(adc_t const *beginning) const;

  // Populates the provided bitset using the 128 bits beginning at the
  // position indicated by the provided pointer
  void fillBitArray(std::bitset<128> &theArray, adc_t const * beginning);

  void printBitArray(std::bitset<128> theArray);

  // Accepts a 128 bit bitset and converts the bits from minIdx to maxIdx
  // into a 16-bit adc_t (minIdx and maxIdx should be within 16 bits of
  // each other as no error-checking is performed at the moment).
  adc_t convertFromBinary(std::bitset<128> theArray, int minIdx, int maxIdx) const;

};

// Convert index in number of DataBlocks to index in bytes
inline size_t mu2e::mu2eFragmentReader::blockIndexBytes(size_t offset) const {
  if(hdr_block_count() == 0) { return 0; }
  else if(offset>=hdr_block_count()) { return -1; }
  else if(offset==0) { return 0;}
  return header_()->index[ offset-1 ];
}

// Return size of all blocks
inline size_t mu2e::mu2eFragmentReader::blockSizeBytes() const {
  return mu2eFragment::dataEndBytes();
}

// Return size of block at given DataBlock index
inline size_t mu2e::mu2eFragmentReader::blockSizeBytes(size_t offset) const {
  if(hdr_block_count() == 0) { return 0; }
  else if(offset > hdr_block_count()-1) { return 0; }
  else if(offset==0) { return header_()->index[ offset ]; }
  return header_()->index[ offset ] - header_()->index[ offset-1 ];
}

// Return size of block at given DataBlock index
inline size_t mu2e::mu2eFragmentReader::blockEndBytes(size_t offset) const {
  if(hdr_block_count() == 0) { return 0; }
  else if(offset > hdr_block_count()-1) { return 0; }
  return header_()->index[ offset ];
}

// Return pointer to beginning of DataBlock at given byte index
inline mu2e::mu2eFragmentReader::Header::data_t const * mu2e::mu2eFragmentReader::dataAtBytes(size_t offset) const {
  return dataBegin() + (offset / sizeof(Header::data_t));
}

// Return pointer to beginning of DataBlock at given DataBlock index
inline mu2e::mu2eFragmentReader::Header::data_t const * mu2e::mu2eFragmentReader::dataAtBlockIndex(size_t offset) const {
  return dataAtBytes(blockIndexBytes(offset));
}



void mu2e::mu2eFragmentReader::printPacketAtByte(size_t offset) const {
  std::bitset<128> theArray = bitArray( reinterpret_cast<mu2e::mu2eFragmentReader::adc_t const *>(dataAtBytes(offset)) );
  std::cout << "\t\t" << "Packet Bits (128): ";
  for(int i=0; i<128; i++) {
    std::cout << theArray[i];
    if(i!=0 && i<128-1 && (i+1)%8==0) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;
  return;
}





std::bitset<128> mu2e::mu2eFragmentReader::bitArray(mu2e::mu2eFragmentReader::adc_t const *beginning) const {
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

void mu2e::mu2eFragmentReader::fillBitArray(std::bitset<128> &theArray, mu2e::mu2eFragmentReader::adc_t const * beginning) {
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

void mu2e::mu2eFragmentReader::printBitArray(std::bitset<128> theArray) {
  // Print all 128 bits in the packet
  std::cout << "\t\t" << "Packet Bits (128): ";
  for(int i=0; i<128; i++) {
    std::cout << theArray[i];
  }
  std::cout << std::endl;
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::convertFromBinary(std::bitset<128> theArray, int minIdx, int maxIdx) const {
  std::bitset<16> retVal;
  for(int i=minIdx+1; i<=maxIdx; i++) {
    retVal.set(maxIdx-i,theArray[i]);
  }
  return retVal.to_ulong();
}


#endif /* mu2e_artdaq_Overlays_mu2eFragmentReader_hh */

