#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_EventHeader_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_EventHeader_h

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace DTCLib {

struct DTC_EventHeader
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

	DTC_EventHeader()
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

	inline std::string toJson() const
	{
		std::ostringstream oss;

		oss << "\"DTC_EventHeader\": {\n";
		oss << "\t\"inclusive_event_byte_count\": " << inclusive_event_byte_count;
		oss << ",\n\t\"event_tag_low\": " << event_tag_low;
		oss << ",\n\t\"event_tag_high\": " << event_tag_high;
		oss << ",\n\t\"num_dtcs\": " << num_dtcs;
		oss << ",\n\t\"event_mode\": 0x" << std::hex << event_mode;
		oss << ",\n\t\"dtc_mac\": " << dtc_mac;
		oss << ",\n\t\"partition_id\": " << partition_id;
		oss << ",\n\t\"evb_mode\": " << evb_mode;
		oss << ",\n\t\"evb_id\": " << evb_id;
		oss << ",\n\t\"evb_status\": " << evb_status;
		oss << ",\n\t\"emtdc\": " << emtdc;
		oss << "\n}";

		return oss.str();
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_EventHeader_h
