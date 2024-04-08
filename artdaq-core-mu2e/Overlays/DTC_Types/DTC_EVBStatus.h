#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_EVBStatus_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_EVBStatus_h

#include <bitset>   // std::bitset
#include <cstdint>  // uint8_t, uint16_t

namespace DTCLib {

struct DTC_EVBStatus
{
	bool error;
	std::bitset<7> flags;

	enum class Flags : uint8_t
	{
		kEventFragmentTimeout = 0,
		kReserved1 = 1,
		kReserved2 = 2,
		kReserved3 = 3,
		kReserved4 = 4,
		kReserved5 = 5,
		kReserved6 = 6,
		kInvalid
	};

	DTC_EVBStatus()
		: error(false), flags(0) {}
	DTC_EVBStatus(uint8_t word)
	{
		error = (word & 0x80) != 0;
		flags = std::bitset<7>(word);
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_EVBStatus_h
