#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_LinkStatus_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_LinkStatus_h

#include <bitset>
#include <cstdint>

namespace DTCLib {

struct DTC_LinkStatus
{
	bool error;
	std::bitset<7> flags;

	enum class Flags : uint8_t
	{
		kROCTimeoutError = 0,
		kReserved1 = 1,
		kPacketSequenceNumberError = 2,
		kPacketCRCError = 3,
		kReserved4 = 4,
		kReserved5 = 5,
		kFatalError = 6,
		kInvalid
	};
	DTC_LinkStatus()
		: error(false), flags(0) {}
	DTC_LinkStatus(uint8_t word)
	{
		error = (word & 0x80) != 0;
		flags = std::bitset<7>(word);
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_LinkStatus_h
