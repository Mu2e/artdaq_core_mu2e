#ifndef mu2e_artdaq_core_Overlays_mu2eFragmentWriter_hh
#define mu2e_artdaq_core_Overlays_mu2eFragmentWriter_hh

////////////////////////////////////////////////////////////////////////
// mu2eFragmentWriter
//
// Class derived from mu2eFragment which allows writes to the data (for
// simulation purposes). Note that for this reason it contains
// non-const members which hide the const members in its parent class,
// mu2eFragment, including its reference to the artdaq::Fragment
// object, artdaq_Fragment_, as well as its functions pointing to the
// beginning and end of ADC values in the fragment, dataBegin() and
// dataEnd()
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "mu2e-artdaq-core/Overlays/mu2eFragment.hh"

#include <iostream>

#include "tracemf.h"

namespace mu2e {
class mu2eFragmentWriter;
}

class mu2e::mu2eFragmentWriter : public mu2e::mu2eFragment {
 public:
  mu2eFragmentWriter(artdaq::Fragment &f);

  // These functions form overload sets with const functions from
  // mu2e::DTCFragment

  Header::data_t *dataBegin();
  Header::data_t *dataAt(size_t index);
  Header::data_t *dataEnd();
  Header::data_t *dataAtBytes(size_t offset);

  // We'll need to hide the const version of header in DTCFragment in
  // order to be able to perform writes

  Header *header_() {
    assert(artdaq_Fragment_.dataSize() >= words_to_frag_words_(Header::size_words));
    return reinterpret_cast<Header *>(&*artdaq_Fragment_.dataBegin());
  }

  void set_hdr_fragment_type(Header::data_t type) { header_()->fragment_type = type; }

  void addSpace(size_t bytes);
  void endSubEvt(size_t bytes);

 private:
  // Note that this non-const reference hides the const reference in the base class
  artdaq::Fragment &artdaq_Fragment_;
  static size_t words_to_frag_words_(size_t nWords);
};

// The constructor will expect the artdaq::Fragment object it's been
// passed to contain the artdaq::Fragment header + the
// DTCFragment::Metadata object, otherwise it throws

mu2e::mu2eFragmentWriter::mu2eFragmentWriter(artdaq::Fragment &f) : mu2eFragment(f), artdaq_Fragment_(f) {
  // If this assert doesn't hold, then can't call
  // "words_to_frag_words_" below, translating between the
  // DTCFragment's standard data type size and the
  // artdaq::Fragment's data type size, on the Metadata object

  TLOG_ARB(TLVL_DEBUG, "mu2eFragmentWriter") << "mu2eFragmentWriter Constructor";

  assert(sizeof(Metadata::data_t) == sizeof(Header::data_t));

  if (artdaq_Fragment_.size() !=
      artdaq::detail::RawFragmentHeader::num_words() + words_to_frag_words_(Metadata::size_words)) {
    std::cerr << "artdaq_Fragment size: " << artdaq_Fragment_.size() << std::endl;
    std::cerr << "Expected size: "
              << artdaq::detail::RawFragmentHeader::num_words() + words_to_frag_words_(Metadata::size_words)
              << std::endl;

    throw cet::exception(
        "mu2eFragmentWriter: Raw artdaq::Fragment object size suggests it does not consist of its own header + the "
        "mu2eFragment::Metadata object");
  }

  // Allocate space for the header
  artdaq_Fragment_.resize(words_to_frag_words_(Header::size_words));
  header_()->block_count = 0;
  memset((void *)&(header_()->index[0]), 0, sizeof(size_t) * mu2e::BLOCK_COUNT_MAX);
}

inline mu2e::mu2eFragmentWriter::Header::data_t *mu2e::mu2eFragmentWriter::dataBegin() {
  assert(artdaq_Fragment_.dataSize() >= words_to_frag_words_(Header::size_words));
  return reinterpret_cast<Header::data_t *>(header_() + 1);
}

inline mu2e::mu2eFragmentWriter::Header::data_t *mu2e::mu2eFragmentWriter::dataAt(size_t index) {
  if (index == 0) return dataBegin();
  auto block = header_()->index[index - 1] / sizeof(Header::data_t);
  return reinterpret_cast<Header::data_t *>(dataBegin() + block);
}

inline mu2e::mu2eFragmentWriter::Header::data_t *mu2e::mu2eFragmentWriter::dataEnd() {
  return dataAt(hdr_block_count());
}

inline mu2e::mu2eFragmentWriter::Header::data_t *mu2e::mu2eFragmentWriter::dataAtBytes(size_t offset) {
  return dataBegin() + (offset / sizeof(Header::data_t));
}

inline size_t mu2e::mu2eFragmentWriter::words_to_frag_words_(size_t nWords) {
  size_t mod = nWords % words_per_frag_word_();
  return mod ? nWords / words_per_frag_word_() + 1 : nWords / words_per_frag_word_();
}

void mu2e::mu2eFragmentWriter::addSpace(size_t bytes) {
  TLOG_ARB(10, "mu2eFragmentWriter") << "addSpace: START bytes=" << bytes;
  auto currSize = sizeof(artdaq::Fragment::value_type) * artdaq_Fragment_.size();
  TLOG_ARB(10, "mu2eFragmentWriter") << "addSpace: Resizing fragment: additional bytes requested: " << bytes
                                     << ", size of fragment: " << currSize;
  artdaq_Fragment_.resizeBytes(bytes + currSize);
}

void mu2e::mu2eFragmentWriter::endSubEvt(size_t bytes) {
  TLOG_ARB(11, "mu2eFragmentWriter") << "endSubEvt START bytes=" << bytes;
  header_()->index[hdr_block_count()] = blockOffset(hdr_block_count()) + bytes;
  header_()->block_count++;
  TLOG_ARB(11, "mu2eFragmentWriter") << "endSubEvt END";
}

#endif /* mu2e_artdaq_core_Overlays_mu2eFragmentWriter_hh */
