#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"
// #include "STMDAQ-TestBeam/utils/Hex.hh"
// #include "/home/mu2estm/STMDAQ-TestBeam/utils/dataVars.hh"
//  #include "STMDAQ-TestBeam/utils/xml.hh"
//  #include "STMDAQ-TestBeam/utils/EnvVars.hh"

// STM-TODO: this is for the simpler sim data we are sending
// STM-TODO: will need to update to use the struct in dataVars.hh
// struct fw_tHdr {
//   // Length in int16_t values of the trigger header
//   static const uint len = 3;
//   // Size in bytes of the trigger header
//   static const uint size = len*sizeof(int16_t);

//   // Index positions of trigger header components
//   static const uint EvNum = 0; // event number
//   static const uint DataType = 1; // raw = 0, ZS = 1
//   static const uint EvLen = 2; // number of int16_ts of data to read

//   // Index position of the data
//   static const uint DataBegin = 3;
// };

// TODO: get this from the authoritative source

namespace mu2e {
class STMFragment
{
public:
  static constexpr size_t RAW_HEADER_LEN = 32; // length in words of RAW header
  static constexpr size_t ZS_HEADER_LEN  = 6; // length in words of ZS header
  static constexpr size_t MWD_HEADER_LEN = 4; // length in words of MWD header

        // static const fw_tHdr tHdr;

        explicit STMFragment(artdaq::Fragment const& f)
                : artdaq_fragment_(f) {}

        int16_t const* GetTHdr() const
        {
                return reinterpret_cast<int16_t const*>(artdaq_fragment_.dataBegin());
        }

        int16_t const* detID() const
        {
                // return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.Ch_DTCclk_0);
                return reinterpret_cast<int16_t const*>(GetTHdr() + 28);
        }

        int16_t const* EvNum() const
        {
                // return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.EvNum_0);
                return reinterpret_cast<int16_t const*>(GetTHdr() + 8);
        }

        int16_t const* DataType() const
        {
                // return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.ZSflag_PreVal);
                // return reinterpret_cast<int16_t const*>(GetTHdr()+22);

                // Dont have data type in Header yet for now assume it is Raw data
                return reinterpret_cast<int16_t const*>(GetTHdr() + 22);
        }

        int16_t const* EvLen() const
        {
                // return reinterpret_cast<int16_t const*>(GetTHdr()+tHdr.EvLen);
                return reinterpret_cast<int16_t const*>(GetTHdr() + 23);
        }

        int16_t const* DataBegin() const
        {
                return reinterpret_cast<int16_t const*>(GetTHdr() + 32);
        }

private:
        artdaq::Fragment const& artdaq_fragment_;
};

}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
