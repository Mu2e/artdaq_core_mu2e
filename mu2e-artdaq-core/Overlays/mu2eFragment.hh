#ifndef mu2e_artdaq_core_Overlays_mu2eFragment_hh
#define mu2e_artdaq_core_Overlays_mu2eFragment_hh

#include "artdaq-core/Data/Fragment.hh"
//#include "artdaq/DAQdata/features.hh"
#include "cetlib/exception.h"

#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"
#include "trace.h"

#include <ostream>
#include <vector>

// Implementation of "mu2eFragment", an artdaq::Fragment overlay class

// The "packing factor": How many DataBlocks are stored in each mu2eFragment
#define DATA_BLOCKS_PER_MU2E_FRAGMENT 10000

namespace mu2e {
  class mu2eFragment;

  static const int BLOCK_COUNT_MAX = DATA_BLOCKS_PER_MU2E_FRAGMENT;

  // Let the "<<" operator dump the mu2eFragment's data to stdout
  std::ostream & operator << (std::ostream &, mu2eFragment const &);
}

class mu2e::mu2eFragment {
  public:

  // The mu2eFragment represents its data through the adc_t type, which
  // is a typedef of an 16-member uint8_t array.

  //typedef uint8_t packet_t[16];

  // The "Metadata" struct is used to store info primarily related to
  // the upstream hardware environment from where the fragment came

  // "data_t" is a typedef of the fundamental unit of data the
  // metadata structure thinks of itself as consisting of; it can give
  // its size via the static "size_words" variable (
  // mu2eFragment::Metadata::size_words )

  struct Metadata {

    typedef uint8_t data_t;
    typedef uint32_t run_number_t;

    data_t sim_mode : 4;
    data_t unused : 4; // 4 + 4 = 8 bits

    data_t board_id;
    data_t unused2;
    data_t unused3;
    
    run_number_t run_number;

    static size_t const size_words = 8ul; // Units of Metadata::data_t
  };

  static_assert (sizeof (Metadata) == Metadata::size_words * sizeof (Metadata::data_t), "mu2eFragment::Metadata size changed");


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
    typedef uint64_t count_t;
    
    count_t     block_count : 60;    
    count_t     fragment_type : 4;

	size_t index[DATA_BLOCKS_PER_MU2E_FRAGMENT];

    static size_t const size_words = 8ul + DATA_BLOCKS_PER_MU2E_FRAGMENT * sizeof(size_t); // Units of Header::data_t
  };

  static_assert (sizeof (Header) == sizeof(Header::data_t) * Header::size_words, "mu2eFragment::Header: incorrect size");

  // The constructor simply sets its const private member "artdaq_Fragment_"
  // to refer to the artdaq::Fragment object

  mu2eFragment(artdaq::Fragment const & f ) : artdaq_Fragment_(f) {}

  // const getter functions for the data in the header
  Header::count_t hdr_block_count() const { return header_()->block_count; }
  Header::data_t hdr_fragment_type() const { return (Header::data_t)header_()->fragment_type; }

  static constexpr size_t hdr_size_words() { return Header::size_words; }
 
  // Start of the DTC packets, returned as a pointer to the packet type
  packet_t const * dataBegin() const {
    return reinterpret_cast<packet_t const *>(header_() + 1);
  }

  packet_t const * dataEnd() const {
    if(hdr_block_count() == 0) { return dataBegin(); }
    auto frag = header_()->index[ hdr_block_count() - 1];
    return reinterpret_cast<packet_t const *>((uint8_t*)dataBegin() + frag);
  }

  size_t dataSize() const {
    TRACE(4, "hdr_block_count() == %lu", hdr_block_count());
    if(hdr_block_count() == 0) { return 0; }
    return header_()->index[ hdr_block_count() - 1];
  }

  size_t fragSize() const { return artdaq_Fragment_.size(); }

  protected:

  static constexpr size_t words_per_frag_word_() {
    return sizeof(artdaq::Fragment::value_type) / sizeof(Header::data_t);
  }

  // header_() simply takes the address of the start of this overlay's
  // data (i.e., where the mu2eFragment::Header object begins) and
  // casts it as a pointer to mu2eFragment::Header

  Header const * header_() const {
    return reinterpret_cast<mu2eFragment::Header const *>(&*artdaq_Fragment_.dataBegin());
  }

private:

  artdaq::Fragment const & artdaq_Fragment_;
};

#endif /* mu2e_artdaq_Overlays_mu2eFragment_hh */
