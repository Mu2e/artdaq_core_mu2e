#ifndef mu2e_artdaq_core_Overlays_mu2eFragmentWriter_hh
#define mu2e_artdaq_core_Overlays_mu2eFragmentWriter_hh

////////////////////////////////////////////////////////////////////////
// mu2eFragmentWriter
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
#include "artdaq-core/Data/Fragments.hh"
#include "mu2e-artdaq-core/Overlays/mu2eFragment.hh"

#include <iostream>

namespace mu2e {
  class mu2eFragmentWriter;
}


class mu2e::mu2eFragmentWriter: public mu2e::mu2eFragment {
public:


  mu2eFragmentWriter(artdaq::Fragment & f); 

  // These functions form overload sets with const functions from
  // mu2e::DTCFragment

  DTCFragment * dataBegin();
  DTCFragment * dataEnd();

  // We'll need to hide the const version of header in DTCFragment in
  // order to be able to perform writes

  Header * header_() {
    assert(artdaq_Fragment_.dataSize() >= Header::size_words );
    return reinterpret_cast<Header *>(&*artdaq_Fragment_.dataBegin());
  }

  void set_hdr_fragment_type(Header::data_t type) {
    header_()->fragment_type = type;
  }

  void addDTCFragment(DTCFragment* frag);
  void addDTCFragments(DTCFragment* frags, int count);
  void addDTCFragments(artdaq::FragmentPtrs* frags, int count);

private:
  // Note that this non-const reference hides the const reference in the base class
  artdaq::Fragment & artdaq_Fragment_;
};

// The constructor will expect the artdaq::Fragment object it's been
// passed to contain the artdaq::Fragment header + the
// DTCFragment::Metadata object, otherwise it throws

mu2e::mu2eFragmentWriter::mu2eFragmentWriter(artdaq::Fragment& f ) :
  mu2eFragment(f), artdaq_Fragment_(f) {
   
    // If this assert doesn't hold, then can't call
    // "words_to_frag_words_" below, translating between the
    // DTCFragment's standard data type size and the
    // artdaq::Fragment's data type size, on the Metadata object

    assert( sizeof(Metadata::data_t) == sizeof(Header::data_t) );

 
    if (artdaq_Fragment_.size() != 
	artdaq::detail::RawFragmentHeader::num_words() + Metadata::size_words )
      {
	std::cerr << "artdaq_Fragment size: " << artdaq_Fragment_.size() << std::endl;
	std::cerr << "Expected size: " << artdaq::detail::RawFragmentHeader::num_words() + 
	 Metadata::size_words << std::endl;

	throw cet::exception("mu2eFragmentWriter: Raw artdaq::Fragment object size suggests it does not consist of its own header + the mu2eFragment::Metadata object");
      }
 
    // Allocate space for the header
	// No conversion needed since the basic unit of data is one byte
    artdaq_Fragment_.resize( Header::size_words );
}


inline mu2e::DTCFragment * mu2e::mu2eFragmentWriter::dataBegin() {
  assert(artdaq_Fragment_.dataSize() > Header::size_words);
  return reinterpret_cast<DTCFragment *>(header_() + 1);
}

inline mu2e::DTCFragment * mu2e::mu2eFragmentWriter::dataEnd() {
  return header_()->fragments[header_()->fragment_count - 1] +
	sizeof(header_()->fragments[header_()->fragment_count - 1]);
}


inline void mu2e::mu2eFragmentWriter::addDTCFragment(DTCFragment* frag) {
  if(header_()->fragment_count < mu2e::DTC_FRAGMENT_MAX) {
	header_()->fragments[header_()->fragment_count] = dataEnd();
    artdaq_Fragment_.resizeBytes(sizeof(frag) + sizeof(*this));
	memcpy(dataEnd(), frag, sizeof(*frag));
    header_()->fragment_count++;
  }
}

inline void mu2e::mu2eFragmentWriter::addDTCFragments(DTCFragment* frags, int count) {
  if(header_()->fragment_count + count <= mu2e::DTC_FRAGMENT_MAX) {
    size_t size = 0;
	for(int i = 0; i < count; ++i) {
	  header_()->fragments[header_()->fragment_count + i] = dataEnd() + size;
      size += sizeof(frags[i]);
    }
    artdaq_Fragment_.resizeBytes(sizeof(*this) + sizeof(frags));
	memcpy(dataEnd(), frags, size);
    header_()->fragment_count += count;
  }
}

void mu2e::mu2eFragmentWriter::addDTCFragments(artdaq::FragmentPtrs* frags, int count) {
  if(header_()->fragment_count + count <= mu2e::DTC_FRAGMENT_MAX) {
	for(int i = 0; i < count; ++i) {
	  addDTCFragment((DTCFragment*)((*frags)[i].get()));
    }
  }
}

#endif /* mu2e_artdaq_core_Overlays_mu2eFragmentWriter_hh */
