#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_ROC_Emulation_Type_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_ROC_Emulation_Type_h

#include <cstdint>

namespace DTCLib {

enum DTC_ROC_Emulation_Type : uint8_t
{
	ROC_Internal_Emulation = 0,
	ROC_FiberLoopback_Emulation = 1,
	ROC_External_Emulation = 2
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_ROC_Emulation_Type_h
