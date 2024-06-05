#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_Link_ID_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_Link_ID_h

#include <cstdint>  // uint8_t, uint16_t
#include <ostream>
#include <vector>  // std::vector

namespace DTCLib {

enum DTC_Link_ID : uint8_t
{
	DTC_Link_0 = 0,
	DTC_Link_1 = 1,
	DTC_Link_2 = 2,
	DTC_Link_3 = 3,
	DTC_Link_4 = 4,
	DTC_Link_5 = 5,
	DTC_Link_CFO = 6,
	DTC_Link_EVB = 7,
	DTC_Link_Unused,
	DTC_Link_ALL = 255
};

inline std::ostream& operator<<(std::ostream& o, DTC_Link_ID const& link)
{
	return o << static_cast<unsigned int>(link);
}

static const std::vector<DTC_Link_ID> DTC_ROC_Links{DTC_Link_0,
													DTC_Link_1,
													DTC_Link_2,
													DTC_Link_3,
													DTC_Link_4,
													DTC_Link_5};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_Link_ID_h
