#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_SubEventHeader_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_SubEventHeader_h

#include <cstdint>
#include <iomanip>
#include <sstream>

// For generated DTC_SubEvent objects
#define CURRENT_SUBEVENT_FORMAT_VERSION 1

namespace DTCLib {

struct DTC_SubEventHeader
{
	uint64_t inclusive_subevent_byte_count : 25;
	uint64_t reserved1 : 7;
	uint64_t event_tag_low : 32;

	uint64_t event_tag_high : 16;
	uint64_t num_rocs : 8;
	uint64_t event_mode : 40;

	uint64_t dtc_mac : 8;
	uint64_t partition_id : 8;
	uint64_t evb_mode : 8;
	uint64_t source_dtc_id : 8;
	uint64_t source_subsystem : 3;
	uint64_t reserved2 : 29;

	uint64_t link0_status : 8;
	uint64_t link1_status : 8;
	uint64_t link2_status : 8;
	uint64_t link3_status : 8;
	uint64_t link4_status : 8;
	uint64_t link5_status : 8;
	uint64_t subevent_format_version : 8;
	uint64_t emtdc : 8;

	uint64_t link4_drp_rx_latency : 16;
	uint64_t link5_drp_rx_latency : 16;
	uint64_t reserved3 : 32;
	uint64_t link0_drp_rx_latency : 16;
	uint64_t link1_drp_rx_latency : 16;
	uint64_t link2_drp_rx_latency : 16;
	uint64_t link3_drp_rx_latency : 16;

	

	DTC_SubEventHeader()
		: inclusive_subevent_byte_count(0)
		, reserved1(0)
		, event_tag_low(0)
		, event_tag_high(0)
		, num_rocs(0)
		, event_mode(0)
		, dtc_mac(0)
		, partition_id(0)
		, evb_mode(0)
		, source_dtc_id(0)
		, reserved2(0)
		, link0_status(0)
		, link1_status(0)
		, link2_status(0)
		, link3_status(0)
		, link4_status(0)
		, link5_status(0)
		, subevent_format_version(CURRENT_SUBEVENT_FORMAT_VERSION)
		, emtdc(0)
		, link4_drp_rx_latency(0)
		, link5_drp_rx_latency(0)
		, reserved3(0)
		, link0_drp_rx_latency(0)
		, link1_drp_rx_latency(0)
		, link2_drp_rx_latency(0)
		, link3_drp_rx_latency(0)
	{}

	inline std::string toJson() const
	{
		std::ostringstream oss;

		oss << "\"DTC_SubEventHeader\": {\n";
		oss << "\t\"inclusive_subevent_byte_count\": " << inclusive_subevent_byte_count;
		oss << ",\n\t\"event_tag_low\": " << event_tag_low;
		oss << ",\n\t\"event_tag_high\": " << event_tag_high;
		oss << ",\n\t\"num_rocs\": " << num_rocs;
		oss << ",\n\t\"event_mode\": 0x" << std::hex << event_mode << std::dec;
		oss << ",\n\t\"dtc_mac\": " << dtc_mac;
		oss << ",\n\t\"partition_id\": " << partition_id;
		oss << ",\n\t\"evb_mode\": " << evb_mode;
		oss << ",\n\t\"source_dtc_id\": " << source_dtc_id;
		oss << ",\n\t\"link0_status\": " << link0_status;
		oss << ",\n\t\"link1_status\": " << link1_status;
		oss << ",\n\t\"link2_status\": " << link2_status;
		oss << ",\n\t\"link3_status\": " << link3_status;
		oss << ",\n\t\"link4_status\": " << link4_status;
		oss << ",\n\t\"link5_status\": " << link5_status;
		oss << ",\n\t\"subevent_format_version\": " << subevent_format_version;
		oss << ",\n\t\"emtdc\": " << emtdc;
		oss << ",\n\t\"link0_drp_rx_latency\": " << link0_drp_rx_latency;
		oss << ",\n\t\"link1_drp_rx_latency\": " << link1_drp_rx_latency;
		oss << ",\n\t\"link2_drp_rx_latency\": " << link2_drp_rx_latency;
		oss << ",\n\t\"link3_drp_rx_latency\": " << link3_drp_rx_latency;
		oss << ",\n\t\"link4_drp_rx_latency\": " << link4_drp_rx_latency;
		oss << ",\n\t\"link5_drp_rx_latency\": " << link5_drp_rx_latency;
		oss << "\n}";

		return oss.str();
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_SubEventHeader_h
