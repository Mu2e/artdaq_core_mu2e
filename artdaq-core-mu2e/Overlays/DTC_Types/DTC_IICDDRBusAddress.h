#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_IICDDRBusAddress_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_IICDDRBusAddress_h

#include <cstdint>  // uint8_t, uint16_t

namespace DTCLib {

enum DTC_IICDDRBusAddress : uint8_t
{
	DTC_IICDDRBusAddress_DDROscillator = 0x59,
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_IICDDRBusAddress_h
