#ifndef artdaq_core_mu2e_Overlays_CFO_Packets_CFO_EventHeader_h
#define artdaq_core_mu2e_Overlays_CFO_Packets_CFO_EventHeader_h

#include <cstdint>  // uint8_t, uint16_t
#include <string>

namespace CFOLib {

struct CFO_EventHeader
{
	uint64_t inclusive_event_byte_count : 24;
	uint64_t reserved1 : 8;
	uint64_t event_tag_low : 32;

	uint64_t event_tag_high : 16;
	uint64_t num_dtcs : 8;
	uint64_t event_mode : 40;

	uint64_t dtc_mac : 8;
	uint64_t partition_id : 8;
	uint64_t evb_mode : 8;
	uint64_t evb_id : 8;
	uint64_t evb_status : 8;
	uint64_t emtdc : 8;
	uint64_t reserved2 : 16;

	CFO_EventHeader()
		: inclusive_event_byte_count(0)
		, reserved1(0)
		, event_tag_low(0)
		, event_tag_high(0)
		, num_dtcs(0)
		, event_mode(0)
		, dtc_mac(0)
		, partition_id(0)
		, evb_mode(0)
		, evb_id(0)
		, evb_status(0)
		, emtdc(0)
		, reserved2(0)
	{}

	std::string toJson() const;
};

}  // namespace CFOLib

#endif  // artdaq_core_mu2e_Overlays_CFO_Packets_CFO_EventHeader_h
