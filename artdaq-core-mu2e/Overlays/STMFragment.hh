#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"
#include <cstdint>
#include <cstddef>

namespace stm {

// ---------------------------
// Dataset identifiers
// ---------------------------
enum class Dataset : uint16_t {
  RAW = 100,
  ZS  = 101,
  MWD = 102
};

// ---------------------------
// RAW header layout
// ---------------------------
struct RawHeader {
  static constexpr size_t WORDS = 21;
  static constexpr uint16_t ANCHOR_WORD = 0xCAFE;

  enum Index : size_t {
    ANCHOR_START = 0,

    EWT_0 = 1,
    EWT_1 = 2,
    EWT_2 = 3,

    ADCclk_0 = 4,
    ADCclk_1 = 5,
    ADCclk_2 = 6,
    ADCclk_3 = 7,

    Ch_DTCclk_0 = 8,
    DTCclk_1 = 9,
    DTCclk_2 = 10,
    DTCclk_3 = 11,

    EM_0 = 12,
    EM_1 = 13,
    EM_2_DRTDC = 14,

    PRESCALE = 15,
    RAW_LEN = 16,
    ZS_REGIONS = 17,
    ZS_LEN = 18,
    PH_NUM = 19,

    ANCHOR_END = 20
  };
};

} // namespace stm

namespace mu2e {

class STMFragment {
public:
  explicit STMFragment(artdaq::Fragment const& f)
    : frag_(f),
      data_(reinterpret_cast<int16_t const*>(f.dataBegin()))
  {}

  // -----------------------
  // Dataset (authoritative)
  // -----------------------
  stm::Dataset dataset() const {
    return static_cast<stm::Dataset>(frag_.fragmentID());
  }

  bool isRaw() const { return dataset() == stm::Dataset::RAW; }
  bool isZS()  const { return dataset() == stm::Dataset::ZS; }
  bool isMWD() const { return dataset() == stm::Dataset::MWD; }

  // -----------------------
  // Header integrity
  // -----------------------
  bool hasValidAnchors() const {
    if (!isRaw()) return true; // No header for ZS/MWD
    return data_[stm::RawHeader::ANCHOR_START] == stm::RawHeader::ANCHOR_WORD &&
           data_[stm::RawHeader::ANCHOR_END]   == stm::RawHeader::ANCHOR_WORD;
  }

  // -----------------------
  // RAW header access
  // -----------------------
  uint64_t eventWindowTag() const {
    return uint64_t(data_[stm::RawHeader::EWT_0]) |
           (uint64_t(data_[stm::RawHeader::EWT_1]) << 16) |
           (uint64_t(data_[stm::RawHeader::EWT_2]) << 32);
  }

  uint64_t adcClock() const {
    return uint64_t(data_[stm::RawHeader::ADCclk_0]) |
           (uint64_t(data_[stm::RawHeader::ADCclk_1]) << 16) |
           (uint64_t(data_[stm::RawHeader::ADCclk_2]) << 32) |
           (uint64_t(data_[stm::RawHeader::ADCclk_3]) << 48);
  }

  uint64_t dtcClock() const {
    return uint64_t(data_[stm::RawHeader::Ch_DTCclk_0]) |
           (uint64_t(data_[stm::RawHeader::DTCclk_1]) << 16) |
           (uint64_t(data_[stm::RawHeader::DTCclk_2]) << 32) |
           (uint64_t(data_[stm::RawHeader::DTCclk_3]) << 48);
  }

  uint16_t rawLength() const {
    return data_[stm::RawHeader::RAW_LEN];
  }

  uint16_t zsRegions() const {
    return data_[stm::RawHeader::ZS_REGIONS];
  }

  uint16_t zsLength() const {
    return data_[stm::RawHeader::ZS_LEN];
  }

  uint16_t prescale() const {
    return data_[stm::RawHeader::PRESCALE];
  }

  // -----------------------
  // Payload access
  // -----------------------
  int16_t const* payloadBegin() const {
    return isRaw() ? data_ + stm::RawHeader::WORDS : data_;
  }

  size_t payloadWords() const {
    return isRaw() ? rawLength()
                   : frag_.dataSizeBytes() / sizeof(int16_t);
  }

private:
  artdaq::Fragment const& frag_;
  int16_t const* data_;
};

} // namespace mu2e

#endif //MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
