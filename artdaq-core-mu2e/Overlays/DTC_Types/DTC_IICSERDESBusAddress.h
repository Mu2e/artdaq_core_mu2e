#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_IICSERDESBusAddress_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_IICSERDESBusAddress_h

#include <cstdint>  // uint8_t, uint16_t

namespace DTCLib {

enum DTC_IICSERDESBusAddress : uint8_t
{
	DTC_IICSERDESBusAddress_EVB = 0x55,
	DTC_IICSERDESBusAddress_CFO = 0x5d,
	DTC_IICSERDESBusAddress_JitterAttenuator = 0x68,
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_IICSERDESBusAddress_h
