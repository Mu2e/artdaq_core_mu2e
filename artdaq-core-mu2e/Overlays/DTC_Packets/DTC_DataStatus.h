#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataStatus_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataStatus_h

#include <cstdint>

namespace DTCLib {

/// <summary>
/// Possible values for the Status word of the Data Header packet
//Bit Position	Definition
// 0	“Event Window has Data” flag indicates detector data present, else No Data for Event Window.
// 1	“Invalid Event Window Request” flag indicates the ROC did not receive a Heartbeat packet corresponding to this Data Request.
// 2	“I am corrupt” flag indicates the ROC has lost data or the ability to conduct detector readout has been compromised.
// 3	“Timeout” flag indicates ROC retrieval of data did not respond before timeout occurred.
// 4	“Overflow” flag indicates data is good, but not all data could be sent.
// 7:5	Reserved
/// </summary>
static const uint8_t DTC_DataStatus_Valid = 0;
// enum DTC_DataStatus //It is a bit mask, can implement easily with enum
// {
// 	DTC_DataStatus_Valid = 0,
// 	DTC_DataStatus_NoValid = 1,
// 	DTC_DataStatus_Invalid = 2,
// };

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataStatus_h
