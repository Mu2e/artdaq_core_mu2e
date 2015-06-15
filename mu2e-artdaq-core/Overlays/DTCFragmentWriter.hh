#ifndef mu2e_artdaq_core_Overlays_DTCFragmentWriter_hh
#define mu2e_artdaq_core_Overlays_DTCFragmentWriter_hh

////////////////////////////////////////////////////////////////////////
// DTCFragmentWriter
//
// Class derived from DTCFragment which allows writes to the data (for
// simulation purposes). Note that for this reason it contains
// non-const members which hide the const members in its parent class,
// DTCFragment, including its reference to the artdaq::Fragment
// object, artdaq_Fragment_, as well as its functions pointing to the
// beginning and end of ADC values in the fragment, dataBegin() and
// dataEnd()
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"

#include <iostream>

namespace mu2e {
  class DTCFragmentWriter;
}


class mu2e::DTCFragmentWriter: public mu2e::DTCFragment {
public:


  DTCFragmentWriter(artdaq::Fragment & f); 

  // These functions form overload sets with const functions from
  // mu2e::DTCFragment

  packet_t * dataBegin();
  packet_t * dataEnd();

  // We'll need to hide the const version of header in DTCFragment in
  // order to be able to perform writes

  Header * header_() {
    assert(artdaq_Fragment_.dataSize() >= words_to_frag_words_(Header::size_words ));
    return reinterpret_cast<Header *>(&*artdaq_Fragment_.dataBegin());
  }

  void set_hdr_timestamp(Header::timestamp_t timestamp) {
    header_()->timestamp = timestamp;
  }

  void resize(size_t nPackets);

private:
  size_t calc_event_size_words_(size_t nPackets);

  static size_t packets_to_words_(size_t nPackets);
  static size_t words_to_frag_words_(size_t nWords);

  // Note that this non-const reference hides the const reference in the base class
  artdaq::Fragment & artdaq_Fragment_;
};

// The constructor will expect the artdaq::Fragment object it's been
// passed to contain the artdaq::Fragment header + the
// DTCFragment::Metadata object, otherwise it throws

mu2e::DTCFragmentWriter::DTCFragmentWriter(artdaq::Fragment& f ) :
  DTCFragment(f), artdaq_Fragment_(f) {
   
    // If this assert doesn't hold, then can't call
    // "words_to_frag_words_" below, translating between the
    // DTCFragment's standard data type size and the
    // artdaq::Fragment's data type size, on the Metadata object

    assert( sizeof(Metadata::data_t) == sizeof(Header::data_t) );

 
    if (artdaq_Fragment_.size() != 
	artdaq::detail::RawFragmentHeader::num_words() + 
	words_to_frag_words_( Metadata::size_words ))
      {
	std::cerr << "artdaq_Fragment size: " << artdaq_Fragment_.size() << std::endl;
	std::cerr << "Expected size: " << artdaq::detail::RawFragmentHeader::num_words() + 
	  words_to_frag_words_( Metadata::size_words) << std::endl;

	throw cet::exception("DTCFragmentWriter: Raw artdaq::Fragment object size suggests it does not consist of its own header + the DTCFragment::Metadata object");
      }
 
    // Allocate space for the header
    artdaq_Fragment_.resize( words_to_frag_words_(Header::size_words) );
}


inline mu2e::DTCFragment::packet_t * mu2e::DTCFragmentWriter::dataBegin() {
  assert(artdaq_Fragment_.dataSize() > words_to_frag_words_(Header::size_words));
  return reinterpret_cast<packet_t *>(header_() + 1);
}

inline mu2e::DTCFragment::packet_t * mu2e::DTCFragmentWriter::dataEnd() {
  return dataBegin() + hdr_packet_count();
}


inline void mu2e::DTCFragmentWriter::resize(size_t nPackets) {
  auto es(calc_event_size_words_(nPackets));
  artdaq_Fragment_.resize(words_to_frag_words_(es));
  header_()->event_size = nPackets;
}

inline size_t mu2e::DTCFragmentWriter::calc_event_size_words_(size_t nPackets) {
  return packets_to_words_(nPackets) + hdr_size_words();
}

inline size_t mu2e::DTCFragmentWriter::packets_to_words_(size_t nPackets) {
  return nPackets * words_per_packet_();
}

inline size_t mu2e::DTCFragmentWriter::words_to_frag_words_(size_t nWords) {
  size_t mod = nWords % words_per_frag_word_();
  return mod ?
    nWords / words_per_frag_word_() + 1 :
    nWords / words_per_frag_word_();
}

#endif /* mu2e_artdaq_core_Overlays_DTCFragmentWriter_hh */
