#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"
//#include "STMDAQ-TestBeam/utils/Hex.hh"
//#include "STMDAQ-TestBeam/utils/dataVars.hh"
//#include "STMDAQ-TestBeam/utils/xml.hh"
//#include "STMDAQ-TestBeam/utils/EnvVars.hh"

// STM-TODO: this is for the simpler sim data we are sending
// STM-TODO: will need to update to use the struct in dataVars.hh
struct fw_tHdr {
  // Length in int16_t values of the trigger header
  static const uint len = 3;
  // Size in bytes of the trigger header
  static const uint size = len*sizeof(int16_t);

  // Index positions of trigger header components
  static const uint EvNum = 0; // event number
  static const uint DataType = 1; // raw = 0, ZS = 1
  static const uint EvLen = 2; // number of int16_ts of data to read

  // Index position of the data
  static const uint DataBegin = 3;
};

namespace mu2e {
  class STMFragment
  {
  public:
    static const fw_tHdr tHdr;
    
    explicit STMFragment(artdaq::Fragment const& f)
      : artdaq_fragment_(f) {}
    
    
    int16_t const* GetTHdr() const {
      return reinterpret_cast<int16_t const*>(artdaq_fragment_.dataBegin());
    }

    int16_t const* EvNum() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.EvNum);
    }

    int16_t const* DataType() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.DataType);
    }

    int16_t const* EvLen() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.EvLen);
    }

    int16_t const* DataBegin() const {
      return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.DataBegin);// + fw_tHdr_Len);
    }
    
  private:
    artdaq::Fragment const& artdaq_fragment_;
  };

}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
