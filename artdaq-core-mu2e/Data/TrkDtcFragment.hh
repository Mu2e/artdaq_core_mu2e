
#ifndef ARTDAQ_CORE_MU2E_DATA_TRKDTC_HH
#define ARTDAQ_CORE_MU2E_DATA_TRKDTC_HH

#include "artdaq-core-mu2e/Data/ArtFragment.hh"

namespace mu2e {
  class TrkDtcFragment : public ArtFragment {
    
  public:

    TrkDtcFragment() : ArtFragment() {}
    TrkDtcFragment(const std::vector<uint8_t>& Data) : ArtFragment(Data) {}

//-----------------------------------------------------------------------------
// the first integer word - version
//-----------------------------------------------------------------------------
    int      nReg    () { return (data_.size()/4-1)/2; }

    int      version () { return ((int*) data_.data())[0    ]; }
    uint32_t reg(int i) { return ((int*) data_.data())[2*i+1]; }
    uint32_t val(int i) { return ((int*) data_.data())[2*i+2]; }
  };
}

#endif
