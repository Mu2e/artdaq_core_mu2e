#ifndef mu2e_artdaq_Overlays_DetectorFragment_hh
#define mu2e_artdaq_Overlays_DetectorFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib/exception.h"

#include <iostream>

#include <ostream>
#include <vector>

#include <bitset>

//#include "dtcInterfaceLib/DTC.h"

// Implementation of "DetectorFragment", an artdaq::Fragment overlay class
// used for processing data from the detector. This class provides helper
// functions for parsing bits in DTC packets, as well as functions for
// accessing specific variables stored in DTC header packets.

namespace mu2e {
  class DetectorFragment;

  // Let the "<<" operator dump the DetectorFragment's data to stdout
  std::ostream & operator << (std::ostream &, DetectorFragment const &);
}

class mu2e::DetectorFragment {
  public:

  // The DetectorFragment represents its data through the adc_t type, which
  // is a typedef of an unsigned 16-bit integer.

  typedef uint16_t adc_t;

  // The "Metadata" struct is used to store info primarily related to
  // the upstream hardware environment from where the fragment came

  // "data_t" is a typedef of the fundamental unit of data the
  // metadata structure thinks of itself as consisting of; it can give
  // its size via the static "size_words" variable (
  // DetectorFragment::Metadata::size_words )

  struct Metadata {

    typedef uint32_t data_t;
    //    typedef __uint128_t data_t;

    data_t board_serial_number : 16;
    data_t num_adc_bits : 8;
    data_t unused : 8; // 16 + 8 + 8 == 32 bits
    
    static size_t const size_words = 1ul; // Units of Metadata::data_t
  };

  static_assert (sizeof (Metadata) == Metadata::size_words * sizeof (Metadata::data_t), "DetectorFragment::Metadata size changed");


  // The "Header" struct contains "metadata" specific to the fragment
  // which is not hardware-related

  // Header::data_t -- not to be confused with Metadata::data_t ! --
  // describes the standard size of a data type not just for the
  // header data, but ALSO the physics data beyond it; the size of the
  // header in units of Header::data_t is given by "size_words", and
  // the size of the fragment beyond the header in units of
  // Header::data_t is given by "event_size"

  // Notice only the first 28 bits of the first 32-bit unsigned
  // integer in the Header is used to hold the event_size ; this means
  // that you can't represent a fragment larger than 2**28 units of
  // data_t, or 1,073,741,824 bytes

  struct Header {
    typedef uint32_t data_t;

    typedef uint32_t event_size_t;  
    typedef uint32_t run_number_t;

    event_size_t event_size : 28;
    event_size_t unused_1   :  4;

    run_number_t run_number : 32;

    static size_t const size_words = 2ul; // Units of Header::data_t
  };

  static_assert (sizeof (Header) == Header::size_words * sizeof (Header::data_t), "DetectorFragment::Header size changed");

  // The constructor simply sets its const private member "artdaq_Fragment_"
  // to refer to the artdaq::Fragment object

  DetectorFragment(artdaq::Fragment const & f ) :
    current_offset_(0),
    current_offset_index_(0),
    artdaq_Fragment_(f)
    {}
 
  virtual ~DetectorFragment() {}

  // const getter functions for the data in the header

  Header::event_size_t hdr_event_size() const { return header_()->event_size; } 
  Header::run_number_t hdr_run_number() const { return header_()->run_number; }
  static constexpr size_t hdr_size_words() { return Header::size_words; }

  // The number of ADC values describing data beyond the header
  size_t total_adc_values() const {
    return (hdr_event_size() - hdr_size_words()) * adcs_per_word_();
  }

  size_t total_adc_values_in_data_block() const {
    mu2e::DetectorFragment::adc_t packetCount = convertFromBinary(bitArray(dataBlockBegin()),127-43,127-32);
    return 8*(size_t(packetCount) + 1);
  }

  // Start of the ADC values returned as a pointer to the ADC type
  adc_t const * dataBegin() const {
    return (reinterpret_cast<mu2e::DetectorFragment::adc_t const *>(header_() + 1));
  }
  // End of the ADC values, returned as a pointer to the ADC type
  adc_t const * dataEnd() const {
    return dataBegin() + total_adc_values();
  }

  // Start of the ADC values for the current block, returned as a
  // pointer to the 16-bit ADC type
  adc_t const * dataBlockBegin() const {
    return (reinterpret_cast<mu2e::DetectorFragment::adc_t const *>(header_() + 1)) + current_offset_;
  }
  // End of the ADC values for the current data block, returned as
  // a pointer to the 16-bit ADC type
  adc_t const * dataBlockEnd() const {
    return dataBlockBegin() + total_adc_values_in_data_block();
  }

  // Functions to check if any ADC values are corrupt

  // findBadADC() checks to make sure that the ADC type (adc_t) variable
  // holding the ADC value doesn't contain bits beyond the expected
  // range, i.e., can't be evaluated to a larger value than the max
  // permitted ADC value

  adc_t const * findBadADC(int daq_adc_bits) const {
    return std::find_if(dataBegin(), dataEnd(), 
			[&](mu2e::DetectorFragment::adc_t const adc) -> bool { 
			  return (adc >> daq_adc_bits); });
  }

  bool fastVerify(int daq_adc_bits) const {
    return (findBadADC(daq_adc_bits) == dataEnd());
  };

  // Defined in DetectorFragment.cc, this throws if any ADC appears corrupt
  void checkADCData(int daq_adc_bits) const; 


  // Largest ADC value possible
  size_t adc_range(int daq_adc_bits) {
    return (1ul << daq_adc_bits );
  }

  size_t numDataBlocks() {
    
    size_t data_counter = 0;
    size_t numDataBlocks = 0;
    size_t totalPacketsRead = 0;
    for(mu2e::DetectorFragment::adc_t const *curPos = dataBegin();
	curPos != dataEnd();
	curPos+=8) { 
      if(data_counter==0) {

	//	std::cout << "ERR: data_counter==0" << std::endl << std::flush;
	

	// Verify that this is a header block
	mu2e::DetectorFragment::adc_t packetType = convertFromBinary(bitArray(curPos),127-24,127-20);
	mu2e::DetectorFragment::adc_t packetCount = convertFromBinary(bitArray(curPos),127-43,127-32);
	//	mu2e::DetectorFragment::adc_t status = convertFromBinary(bitArray(curPos),127-104,127-96);
	data_counter = packetCount;
	// Increment number of data blocks
	if(packetType==5 || packetType == 0x1 || packetType == 0x2) { // Type 0x1 and 0x2 correspond to debug header packets
	  numDataBlocks++;
	} else {
	  //	  DTCLib::DTC_DataPacket errPacket((char*)curPos);
//	  std::cout << "ERR: Packet Type: " << packetType << std::endl << std::flush;
//	  std::cout << "ERR: Packet Conut: " << packetCount << std::endl << std::flush;
//	  std::cout << "ERR: totalPacketsRead: " << totalPacketsRead << std::endl << std::flush;
//	  std::cout << "ERR: BitArray: " << std::endl << std::flush;
//	  printBitArray(bitArray(curPos));
//	  std::cout << std::flush;
	  throw cet::exception("Error in DetectorFragment: Non-dataheader packet found in dataheader packet location");
	}
      } else {
	data_counter--;
      }
      totalPacketsRead++;
    }
    
    return numDataBlocks;
  }

  size_t offset() {
    return current_offset_;
  }
  
  size_t offsetIndex() {
    return current_offset_index_;
  }

  bool setDataBlockIndex(size_t theIndex) {
    size_t numDB = numDataBlocks();
    if(theIndex<numDB) {
      current_offset_index_ = theIndex;
      current_offset_ = 0;

      size_t blockNum = 0;
      mu2e::DetectorFragment::adc_t const *curPos = dataBegin();
      bool foundBlock = false;
      while(!foundBlock) {
	mu2e::DetectorFragment::adc_t packetCount = convertFromBinary(bitArray(curPos),127-43,127-32);
	if(blockNum==theIndex) {
	  foundBlock = true;
	  //	  current_block_length_ = 8*(size_t(packetCount) + 1)
	    } else {
	  // Jump to the next header packet:
	  // (add 1 for the DTC header packet which is not included in the packet count)
	  curPos += 8*(size_t(packetCount) + 1);

	  current_offset_ += 8*(size_t(packetCount) + 1);

	  blockNum++;
	}
      }
    } else {
      return false;
    }
    return true;
  }
  
  // DTC Header Packet Methods
  adc_t byteCount();
  adc_t rocID();
  adc_t packetType();
  adc_t valid();
  adc_t packetCount();
  adc_t status();
  std::vector<adc_t> timestampVector();
  std::vector<adc_t> dataVector();
  void printDTCHeader();
  virtual void printAll();

  protected:

  // Functions to translate between size (in bytes) of an ADC, size of
  // this fragment overlay's concept of a unit of data (i.e.,
  // Header::data_t) and size of an artdaq::Fragment's concept of a
  // unit of data (the artdaq::Fragment::value_type).

  static constexpr size_t adcs_per_word_() {
    return sizeof(Header::data_t) / sizeof(adc_t);
  }

  static constexpr size_t words_per_frag_word_() {
    return sizeof(artdaq::Fragment::value_type) / sizeof(Header::data_t);
  }

  // header_() simply takes the address of the start of this overlay's
  // data (i.e., where the DetectorFragment::Header object begins) and
  // casts it as a pointer to DetectorFragment::Header

  Header const * header_() const {
    return reinterpret_cast<DetectorFragment::Header const *>(&*artdaq_Fragment_.dataBegin());
  }

  // Bit processing helper methods
  // For now, the focus is on clarity and ease of use, but eventually
  // these sorts of functions will need to be optimized for efficiency

  // bitArray populates a 128-bit bitset using the 128 bits beginning
  // at the position indicated by the provided pointer
  //  std::bitset<128> bitArray(adc_t const * beginning);
  std::bitset<128> bitArray(mu2e::DetectorFragment::adc_t const *beginning) const;

  // Populates the provided bitset using the 128 bits beginning at the
  // position indicated by the provided pointer
  void fillBitArray(std::bitset<128> &theArray, adc_t const * beginning);

  void printBitArray(std::bitset<128> theArray);

  // Accepts a 128 bit biteset and converts the bits from minIdx to maxIdx
  // into a 16-bit adc_t (minIdx and maxIdx should be within 16 bits of
  // each other as no error-checking is performed at the moment).
  mu2e::DetectorFragment::adc_t convertFromBinary(std::bitset<128> theArray, int minIdx, int maxIdx) const;

  // current_offset_ stores the offset of the DataBlock currently being accessed
  size_t current_offset_;
  size_t current_offset_index_;

//  // Length of the current datablock in units of 16-bit adc_t values
//  size_t current_block_length_;

private:

  artdaq::Fragment const & artdaq_Fragment_;

};

#endif /* mu2e_artdaq_Overlays_DetectorFragment_hh */
