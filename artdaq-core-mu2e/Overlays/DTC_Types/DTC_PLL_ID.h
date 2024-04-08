#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_PLL_ID_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_PLL_ID_h

#include <cstdint>
#include <vector>

namespace DTCLib {

enum DTC_PLL_ID : uint8_t
{
	DTC_PLL_Link_0 = 0,
	DTC_PLL_Link_1 = 1,
	DTC_PLL_Link_2 = 2,
	DTC_PLL_Link_3 = 3,
	DTC_PLL_Link_4 = 4,
	DTC_PLL_Link_5 = 5,
	DTC_PLL_CFO_RX = 6,
	DTC_PLL_CFO_TX = 7,
	DTC_PLL_EVB_TXRX = 8,
	DTC_PLL_PunchedClock = 9,
	DTC_PLL_Unused,
};
static const std::vector<DTC_PLL_ID> DTC_PLLs{DTC_PLL_Link_0,
											  DTC_PLL_Link_1,
											  DTC_PLL_Link_2,
											  DTC_PLL_Link_3,
											  DTC_PLL_Link_4,
											  DTC_PLL_Link_5};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_PLL_ID_h
