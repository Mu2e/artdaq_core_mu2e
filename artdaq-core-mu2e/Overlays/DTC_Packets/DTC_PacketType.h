#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_PacketType_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_PacketType_h

#include <cstdint>

namespace DTCLib {

/// <summary>
/// Defined Packet Types for the DTC DMA Protocol
/// </summary>
enum DTC_PacketType : uint8_t
{
	DTC_PacketType_DCSRequest = 0,
	DTC_PacketType_Heartbeat = 1,
	DTC_PacketType_DataRequest = 2,
	DTC_PacketType_DCSReply = 4,
	DTC_PacketType_DataHeader = 5,
	DTC_PacketType_Invalid = 0x10,
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_PacketType_h
