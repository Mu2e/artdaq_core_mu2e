#ifndef mu2e_artdaq_core_Overlays_DTCFragment_hh
#define mu2e_artdaq_core_Overlays_DTCFragment_hh

#include "artdaq-core/Data/Fragment.hh"
//#include "artdaq/DAQdata/features.hh"
#include "cetlib/exception.h"

#include <ostream>
#include <vector>

// Implementation of "DTCFragment", an artdaq::Fragment overlay class
// used for pedagogical purposes

namespace mu2e {
  class DTCFragment;

  // Let the "<<" operator dump the DTCFragment's data to stdout
  std::ostream & operator << (std::ostream &, DTCFragment const &);
}

class mu2e::DTCFragment {
  public:

  // The DTCFragment represents its data through the adc_t type, which
  // is a typedef of an 16-member uint8_t array.

  typedef uint8_t packet_t[16];

  // The "Metadata" struct is used to store info primarily related to
  // the upstream hardware environment from where the fragment came

  // "data_t" is a typedef of the fundamental unit of data the
  // metadata structure thinks of itself as consisting of; it can give
  // its size via the static "size_words" variable (
  // DTCFragment::Metadata::size_words )

  struct Metadata {

    typedef uint8_t data_t;
    typedef char addr_t[64];
    typedef uint32_t run_number_t;

    data_t sim_mode : 4;
    data_t unused : 4; // 4 + 4 = 8 bits

    data_t unused1;
    data_t unused2;
    data_t unused3;   
    
    addr_t hostname;
    run_number_t run_number;

    static size_t const size_words = 72ul; // Units of Metadata::data_t
  };

  static_assert (sizeof (Metadata) == Metadata::size_words * sizeof (Metadata::data_t), "DTCFragment::Metadata size changed");


  // The "Header" struct contains "metadata" specific to the fragment
  // which is not hardware-related

  // Header::data_t -- not to be confused with Metadata::data_t ! --
  // describes the standard size of a data type not just for the
  // header data, but ALSO the physics data beyond it; the size of the
  // header in units of Header::data_t is given by "size_words", and
  // the size of the fragment beyond the header in units of
  // Header::data_t is given by "event_size"

  struct Header {
    typedef uint8_t data_t;

    typedef uint64_t timestamp_t;
    typedef uint32_t data_size_t;
    
    data_size_t event_size;

    timestamp_t timestamp : 48;
    timestamp_t unused : 16;

    
    static size_t const size_words = 16ul; // Units of Header::data_t
  };

  static_assert (sizeof (Header) == Header::size_words * sizeof (Header::data_t), "DTCFragment::Header size changed");

  // The constructor simply sets its const private member "artdaq_Fragment_"
  // to refer to the artdaq::Fragment object

  DTCFragment(artdaq::Fragment const & f ) : artdaq_Fragment_(f) {}

  // const getter functions for the data in the header
  Header::data_size_t hdr_packet_count() const { return header_()->event_size; }
  Header::timestamp_t hdr_timestamp() const { return header_()->timestamp; }

  static constexpr size_t hdr_size_words() { return Header::size_words; }
 
  size_t dataSize() const {
    return hdr_packet_count() * words_per_packet_();
  }

  // Start of the DTC packets, returned as a pointer to the packet type
  packet_t const * dataBegin() const {
    return reinterpret_cast<packet_t const *>(header_() + 1);
  }

  // End of the DTC packets, returned as a pointer to the packet type
  packet_t const * dataEnd() const {
    return dataBegin() + hdr_packet_count();
  }

  protected:

  // Functions to translate between size (in bytes) of a DTC packet, size of
  // this fragment overlay's concept of a unit of data (i.e.,
  // Header::data_t) and size of an artdaq::Fragment's concept of a
  // unit of data (the artdaq::Fragment::value_type).

  static constexpr size_t words_per_packet_() {
    return sizeof(packet_t) / sizeof(Header::data_t);
  }

  static constexpr size_t words_per_frag_word_() {
    return sizeof(artdaq::Fragment::value_type) / sizeof(Header::data_t);
  }

  // header_() simply takes the address of the start of this overlay's
  // data (i.e., where the DTCFragment::Header object begins) and
  // casts it as a pointer to DTCFragment::Header

  Header const * header_() const {
    return reinterpret_cast<DTCFragment::Header const *>(&*artdaq_Fragment_.dataBegin());
  }

private:

  artdaq::Fragment const & artdaq_Fragment_;
};

#endif /* mu2e_artdaq_Overlays_DTCFragment_hh */
