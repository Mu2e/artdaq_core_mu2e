#ifndef mu2e_artdaq_Overlays_DetectorFragment_hh
#define mu2e_artdaq_Overlays_DetectorFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib/exception.h"

#include <iostream>

#include <ostream>
#include <vector>

#include <bitset>

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

  DetectorFragment(artdaq::Fragment const & f ) : artdaq_Fragment_(f) {}

  // const getter functions for the data in the header

  Header::event_size_t hdr_event_size() const { return header_()->event_size; } 
  Header::run_number_t hdr_run_number() const { return header_()->run_number; }
  static constexpr size_t hdr_size_words() { return Header::size_words; }

  // The number of ADC values describing data beyond the header
  size_t total_adc_values() const {
    return (hdr_event_size() - hdr_size_words()) * adcs_per_word_();
  }

  size_t total_adc_values_in_data_block() const {
    // The offset list begins 1 adc_t length away from the end of the header_.
    // Each entry in the list is the number of 128-bit packets in that DataBlock
    // so we multiply by 8 to get the number of adc_t values in the DataBlock
    return 8*(*((reinterpret_cast<adc_t const *>(header_() + 1)) + (1 + current_offset_index_)));
  }

  // Start of the ADC values, returned as a pointer to the ADC type
  //  adc_t const * dataBegin() const {
  adc_t const * dataBegin() const {
    // dataBegin returns the start of the current DataBlock
    // The current_offset_ is in units of adc_t (16 bits)
    return (reinterpret_cast<adc_t const *>(header_() + 1)) + current_offset_;
  }

  // End of the ADC values, returned as a pointer to the ADC type
  adc_t const * dataEnd() const {
    return dataBegin() + total_adc_values_in_data_block();
    //    return dataBegin() + total_adc_values();
  }

  // Functions to check if any ADC values are corrupt

  // findBadADC() checks to make sure that the ADC type (adc_t) variable
  // holding the ADC value doesn't contain bits beyond the expected
  // range, i.e., can't be evaluated to a larger value than the max
  // permitted ADC value

  adc_t const * findBadADC(int daq_adc_bits) const {
    return std::find_if(dataBegin(), dataEnd(), 
			[&](adc_t const adc) -> bool { 
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

  void initializeOffset() {
    current_offset_ = 0;
    return;
  }

  size_t numDataBlocks() {
    return *(reinterpret_cast<adc_t const *>(header_() + 1));
  }

  bool setDataBlockIndex(size_t theIndex) {
    if(theIndex<numDataBlocks()) {
      current_offset_index_ = theIndex;
      if(theIndex==0) {
	current_offset_ = 0;
      } else {
	// The offset list begins 1 position after the end of the header_
	// The first entry in the offset list is actually the second offset
	// (since the first is always 0) so it corresponds to theIndex=1
	current_offset_ = 8*(*((reinterpret_cast<adc_t const *>(header_() + 1)) + (1 + (theIndex-1))));
	// The offset list is stored in units of 128-bit packets while
	// current_offset_ is in units of 16-bit adc_t so a factor of
	// 8 is required to convert between them
      }
      return true;
    } else {
      return false;
    }
  }


  // Offset table generator
  virtual void generateOffsetTable(const std::vector<size_t> dataBlockVec);


  // DTC Header Packet Methods
  adc_t byteCount();
  adc_t rocID();
  adc_t packetType();
  adc_t ringID();
  adc_t valid();
  adc_t packetCount();
  adc_t status();
  std::vector<adc_t> timestampVector();
  std::vector<adc_t> dataVector();
  void printDTCHeader();



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
  std::bitset<128> bitArray(adc_t const * beginning);

  // Populates the provided bitset using the 128 bits beginning at the
  // position indicated by the provided pointer
  void fillBitArray(std::bitset<128> &theArray, adc_t const * beginning);

  void printBitArray(std::bitset<128> theArray);

  // Accepts a 128 bit biteset and converts the bits from minIdx to maxIdx
  // into a 16-bit adc_t (minIdx and maxIdx should be within 16 bits of
  // each other as no error-checking is performed at the moment).
  mu2e::DetectorFragment::adc_t convertFromBinary(std::bitset<128> theArray, int minIdx, int maxIdx);

private:

  // current_offset_ stores the offset of the DataBlock currently being accessed
  size_t current_offset_;
  size_t current_offset_index_;
  artdaq::Fragment const & artdaq_Fragment_;

};

#endif /* mu2e_artdaq_Overlays_DetectorFragment_hh */
