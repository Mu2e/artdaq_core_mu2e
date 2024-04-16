#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_Subsystem_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_Subsystem_h

#include <cstdint>  // uint8_t, uint16_t

namespace DTCLib {

enum DTC_Subsystem : uint8_t
{
	DTC_Subsystem_Tracker = 0,
	DTC_Subsystem_Calorimeter = 1,
	DTC_Subsystem_CRV = 2,
	DTC_Subsystem_Other = 3,
	DTC_Subsystem_STM = 4,
	DTC_Subsystem_ExtMon = 5,
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_Subsystem_h
