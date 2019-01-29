#ifndef mu2e_artdaq_Overlays_DetectorFragmentWriter_hh
#define mu2e_artdaq_Overlays_DetectorFragmentWriter_hh

////////////////////////////////////////////////////////////////////////
// DetectorFragmentWriter
//
// Class derived from DetectorFragment which allows writes to the data (for
// simulation purposes). Note that for this reason it contains
// non-const members which hide the const members in its parent class,
// DetectorFragment, including its reference to the artdaq::Fragment
// object, artdaq_Fragment_, as well as its functions pointing to the
// beginning and end of ADC values in the fragment, dataBegin() and
// dataEnd()
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "mu2e-artdaq-core/Overlays/DetectorFragment.hh"

#include <iostream>

namespace mu2e {
class DetectorFragmentWriter;
}

class mu2e::DetectorFragmentWriter : public mu2e::DetectorFragment {
 public:
  DetectorFragmentWriter(artdaq::Fragment &f);

  virtual ~DetectorFragmentWriter(){};

  // These functions form overload sets with const functions from
  // mu2e::DetectorFragment

  adc_t *dataBegin();
  adc_t *dataEnd();

  // We'll need to hide the const version of header in DetectorFragment in
  // order to be able to perform writes

  Header *header_() {
    assert(artdaq_Fragment_.dataSize() >= words_to_frag_words_(Header::size_words));
    return reinterpret_cast<Header *>(&*artdaq_Fragment_.dataBegin());
  }

  void set_hdr_run_number(Header::run_number_t run_number) { header_()->run_number = run_number; }

  void resize(size_t nAdcs);

  virtual void printAll(){};

  void generateOffsetTable(const std::vector<size_t> dataBlockVec);

 private:
  size_t calc_event_size_words_(size_t nAdcs);

  static size_t adcs_to_words_(size_t nAdcs);
  static size_t words_to_frag_words_(size_t nWords);

  // Note that this non-const reference hides the const reference in the base class
  artdaq::Fragment &artdaq_Fragment_;
};

// The constructor will expect the artdaq::Fragment object it's been
// passed to contain the artdaq::Fragment header + the
// DetectorFragment::Metadata object, otherwise it throws

mu2e::DetectorFragmentWriter::DetectorFragmentWriter(artdaq::Fragment &f) : DetectorFragment(f), artdaq_Fragment_(f) {
  // If this assert doesn't hold, then can't call
  // "words_to_frag_words_" below, translating between the
  // DetectorFragment's standard data type size and the
  // artdaq::Fragment's data type size, on the Metadata object

  assert(sizeof(Metadata::data_t) == sizeof(Header::data_t));

  if (artdaq_Fragment_.size() !=
      artdaq::detail::RawFragmentHeader::num_words() + words_to_frag_words_(Metadata::size_words)) {
    std::cerr << "artdaq_Fragment size: " << artdaq_Fragment_.size() << std::endl;
    std::cerr << "Expected size: "
              << artdaq::detail::RawFragmentHeader::num_words() + words_to_frag_words_(Metadata::size_words)
              << std::endl;

    throw cet::exception(
        "DetectorFragmentWriter: Raw artdaq::Fragment object size suggests it does not consist of its own header + the "
        "DetectorFragment::Metadata object");
  }

  // Allocate space for the header
  artdaq_Fragment_.resize(words_to_frag_words_(Header::size_words));
}

inline mu2e::DetectorFragment::adc_t *mu2e::DetectorFragmentWriter::dataBegin() {
  assert(artdaq_Fragment_.dataSize() > words_to_frag_words_(Header::size_words));
  //  return reinterpret_cast<adc_t *>(header_() + 1);
  return (reinterpret_cast<adc_t *>(header_() + 1)) + current_offset_;
}

inline mu2e::DetectorFragment::adc_t *mu2e::DetectorFragmentWriter::dataEnd() {
  return dataBegin() + total_adc_values_in_data_block();
  //  return dataBegin() + total_adc_values();
}

inline void mu2e::DetectorFragmentWriter::resize(size_t nAdcs) {
  auto es(calc_event_size_words_(nAdcs));
  artdaq_Fragment_.resize(words_to_frag_words_(es));
  header_()->event_size = es;
}

inline size_t mu2e::DetectorFragmentWriter::calc_event_size_words_(size_t nAdcs) {
  return adcs_to_words_(nAdcs) + hdr_size_words();
}

inline size_t mu2e::DetectorFragmentWriter::adcs_to_words_(size_t nAdcs) {
  auto mod(nAdcs % adcs_per_word_());
  return (mod == 0) ? nAdcs / adcs_per_word_() : nAdcs / adcs_per_word_() + 1;
}

inline size_t mu2e::DetectorFragmentWriter::words_to_frag_words_(size_t nWords) {
  size_t mod = nWords % words_per_frag_word_();
  return mod ? nWords / words_per_frag_word_() + 1 : nWords / words_per_frag_word_();
}

inline void mu2e::DetectorFragmentWriter::generateOffsetTable(const std::vector<size_t> dataBlockVec) {
  initializeOffset();

  // The first entry in the adc_t array after the DetectorFragment::Header
  // is the number of offsets

  *(dataBegin()) = (adc_t)dataBlockVec.size();
  for (size_t cur_index = 0, cur_offset = dataBlockVec.size() + 1; cur_index < dataBlockVec.size(); cur_index++) {
    // The first offset is always the position of the end of the offset list (dataBlockVec.size()+1)
    // Include a factor of 8 so that the offsets are in units of 16-bit adc_t values (8 per 128-bit packet)
    cur_offset += 8 * dataBlockVec[cur_index];
    *(dataBegin() + 1 + cur_index) = (adc_t)cur_offset;
  }
  setDataBlockIndex(0);

  return;
}

#endif /* mu2e_artdaq_Overlays_DetectorFragmentWriter_hh */
