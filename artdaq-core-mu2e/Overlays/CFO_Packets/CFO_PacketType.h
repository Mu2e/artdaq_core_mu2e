#ifndef artdaq_core_mu2e_Overlays_CFO_Packets_CFO_PacketType_h
#define artdaq_core_mu2e_Overlays_CFO_Packets_CFO_PacketType_h

#include <cstdint>  // uint8_t, uint16_t

namespace CFOLib {

/// <summary>
/// Defined Packet Types for the CFO DMA Protocol
/// </summary>
enum CFO_PacketType : uint8_t
{
	CFO_PacketType_DataHeader = 5,
	CFO_PacketType_Invalid = 0x10,
};

}  // namespace CFOLib

#endif  // artdaq_core_mu2e_Overlays_CFO_Packets_CFO_PacketType_h
