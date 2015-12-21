#ifndef mu2e_artdaq_core_Overlays_mu2eFragment_hh
#define mu2e_artdaq_core_Overlays_mu2eFragment_hh

#include "artdaq-core/Data/Fragment.hh"
//#include "artdaq/DAQdata/features.hh"
#include "cetlib/exception.h"

#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"

#include <ostream>
#include <vector>

// Implementation of "mu2eFragment", an artdaq::Fragment overlay class

// The "packing factor": How many DTCFragments are stored in each mu2eFragment
#define DTC_FRAGMENTS_PER_MU2E_FRAGMENT 100

namespace mu2e {
  class mu2eFragment;

  static const int DTC_FRAGMENT_MAX = DTC_FRAGMENTS_PER_MU2E_FRAGMENT;

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
    
    data_t       fragment_count = 0;    
    data_t      fragment_type : 4;
    data_t      unused : 4;

    DTCFragment* fragments[DTC_FRAGMENTS_PER_MU2E_FRAGMENT];

    static size_t const size_words = 8ul + DTC_FRAGMENTS_PER_MU2E_FRAGMENT * sizeof(DTCFragment*); // Units of Header::data_t
  };

  static_assert (sizeof (Header) == Header::size_words, "mu2eFragment::Header: incorrect size");

  // The constructor simply sets its const private member "artdaq_Fragment_"
  // to refer to the artdaq::Fragment object

  mu2eFragment(artdaq::Fragment const & f ) : artdaq_Fragment_(f) {}

  // const getter functions for the data in the header
  Header::data_t hdr_fragment_count() const { return header_()->fragment_count; }
  Header::data_t hdr_fragment_type() const { return header_()->fragment_type; }

  static constexpr size_t hdr_size_words() { return Header::size_words; }
 
  // Start of the DTC packets, returned as a pointer to the packet type
  DTCFragment const * dataBegin() const {
    return reinterpret_cast<DTCFragment const *>(header_() + 1);
  }

  DTCFragment const * dataEnd() const {
    return reinterpret_cast<DTCFragment const *>(header_()->fragments[hdr_fragment_count() - 1] 
												 + sizeof(header_()->fragments[hdr_fragment_count() - 1]));
  }

  protected:

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
