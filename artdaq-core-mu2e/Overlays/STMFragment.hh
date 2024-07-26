#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"
//#include "STMDAQ-TestBeam/utils/Hex.hh"
//#include "STMDAQ-TestBeam/utils/dataVars.hh"
//#include "STMDAQ-TestBeam/utils/xml.hh"
//#include "STMDAQ-TestBeam/utils/EnvVars.hh"

namespace mu2e {
  class STMFragment
  {
  public:
    explicit STMFragment(artdaq::Fragment const& f)
      : artdaq_fragment_(f) {}
    
    
    int16_t const* GetTHdr() const {
      return reinterpret_cast<int16_t const*>(artdaq_fragment_.dataBegin());
    }

    int16_t const* EvNum() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+0);
    }

    int16_t const* ZPFlag() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+1);
    }

    int16_t const* DataBegin() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+2);// + fw_tHdr_Len);
    }
    
  private:
    artdaq::Fragment const& artdaq_fragment_;
  };

}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
