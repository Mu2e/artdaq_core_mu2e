#ifndef artdaq_core_mu2e_Overlays_CFO_Packets_CFO_EventRecord_h
#define artdaq_core_mu2e_Overlays_CFO_Packets_CFO_EventRecord_h

#include <cstdint>  // uint8_t, uint16_t
#include <iomanip>
#include <sstream>

namespace CFOLib {

struct CFO_EventRecord
{
// CFO Event Record Format Version (63-56)	Res (55-48)	Event Tag (47-0)
// Reserved (63-32)	Linux Time [s] (31-0)
// Event Duration (63-48)	Event Mode (47-0)
// Reserved (63-8)	DR marker N estimate (7-0)
// Reserved (63-8)	DR marker N+1 estimate (7-0)
// TDC of marker N from on-spill start (63-32)	Reserved (31-8)	DR marker N Measurement (7-0)
// Reserved (63-8)	DR marker N+1 Measurement (7-0)

	uint64_t event_tag : 48;
	uint64_t reserved1 : 8;
	uint64_t record_format_version : 8;

	uint64_t linux_timestamp : 32;
	uint64_t reserved2 : 32;

	uint64_t event_mode : 48;
	uint64_t event_duration : 16;

	uint64_t DR_marker_N_est : 8;
	uint64_t reserved3 : 56;

	uint64_t DR_marker_Nplus1_est : 8;
	uint64_t reserved4 : 56;

	uint64_t DR_marker_N_meas : 8;
	uint64_t reserved5 : 24;
	uint64_t TDC_marker_N_from_spill : 32;

	uint64_t DR_marker_Nplus1_meas : 8;
	uint64_t reserved6 : 56;

	CFO_EventRecord()
		: event_tag(0)
		, reserved1(0)
		, record_format_version(0)
		, linux_timestamp(0)
		, reserved2(0)
		, event_mode(0)
		, event_duration(0)
		, DR_marker_N_est(0)
		, reserved3(0)
		, DR_marker_Nplus1_est(0)
		, reserved4(0)
		, DR_marker_N_meas(0)
		, reserved5(0)
		, TDC_marker_N_from_spill(0)
		, DR_marker_Nplus1_meas(0)
		, reserved6(0)
	{}	//end CFO_EventRecord constructor

	inline std::string toJson() const
	{
		std::ostringstream oss;

		oss << "\"CFO_EventRecord\": {\n";
		oss << "\t\"record_format_version\": 0x" <<  std::hex << record_format_version << std::dec;
		oss << ",\n\t\"event_tag\": " << event_tag << "(0x" <<  std::hex << event_tag << std::dec << ")";
		oss << ",\n\t\"linux_timestamp\": " << linux_timestamp;
		oss << ",\n\t\"event_mode\": 0x" << std::hex << event_mode << std::dec;
		oss << ",\n\t\"event_duration\": " << event_duration;
		oss << ",\n\t\"TDC_marker_N_from_spill\": " << TDC_marker_N_from_spill;
		oss << ",\n\t\"DR_marker_N_est\": " << DR_marker_N_est;
		oss << ",\n\t\"DR_marker_Nplus1_est\": " << DR_marker_Nplus1_est;
		oss << ",\n\t\"DR_marker_N_meas\": " << DR_marker_N_meas;
		oss << ",\n\t\"DR_marker_Nplus1_meas\": " << DR_marker_Nplus1_meas;
		oss << "\n}";

		return oss.str();
	} //end toJson()
};

}  // namespace CFOLib

#endif  // artdaq_core_mu2e_Overlays_CFO_Packets_CFO_EventRecord_h
