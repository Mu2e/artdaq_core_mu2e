#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataRequestPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataRequestPacket_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DMAPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_DebugType.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventWindowTag.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"

#include <cstdint>
#include <string>

namespace DTCLib {

/// <summary>
/// The DTC Data Request Packet
/// </summary>
class DTC_DataRequestPacket : public DTC_DMAPacket
{
public:
	/// <summary>
	/// Construct a DTC_DataRequestPacket
	/// </summary>
	/// <param name="link">Destination Link</param>
	/// <param name="debug">Debug Mode flag (Default: true)</param>
	/// <param name="debugPacketCount">Debug Packet Count (Default: 0)</param>
	/// <param name="type">Debug Type (Default: DTC_DebugType_SpecialSequence</param>
	DTC_DataRequestPacket(DTC_Link_ID link, bool debug = true, uint16_t debugPacketCount = 0,
						  DTC_DebugType type = DTC_DebugType_SpecialSequence);
	/// <summary>
	/// Construct a DTC_DataRequestPacket
	/// </summary>
	/// <param name="link">Destination Link</param>
	/// <param name="event_tag">Timestamp to request data for</param>
	/// <param name="debug">Debug Mode flag (Default: true)</param>
	/// <param name="debugPacketCount">Debug Packet Count (Default: 0)</param>
	/// <param name="type">Debug Type (Default: DTC_DebugType_SpecialSequence</param>
	DTC_DataRequestPacket(DTC_Link_ID link, DTC_EventWindowTag event_tag, bool debug = true, uint16_t debugPacketCount = 0,
						  DTC_DebugType type = DTC_DebugType_SpecialSequence);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="right">DTC_DataRequestPacket to copy</param>
	DTC_DataRequestPacket(const DTC_DataRequestPacket& right) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="right">DTC_DataRequestPacket to move</param>
	DTC_DataRequestPacket(DTC_DataRequestPacket&& right) = default;
	/// <summary>
	/// Construct a DTC_DataRequestPacket from the given DTC_DataPacket
	/// </summary>
	/// <param name="in">DTC_DataPacket to overlay</param>
	explicit DTC_DataRequestPacket(const DTC_DataPacket in);

	/// <summary>
	/// Get the value of the debug flag
	/// </summary>
	/// <returns>Debug Flag value</returns>
	bool GetDebug() const { return debug_; }

	/// <summary>
	/// Get the Debug type
	/// </summary>
	/// <returns>DTC_DebugType enumeration value</returns>
	DTC_DebugType GetDebugType() const { return type_; }

	/// <summary>
	/// Get the Debug Packet Count
	/// </summary>
	/// <returns>Number of packets requested by Data Request</returns>
	uint16_t GetDebugPacketCount() const { return debugPacketCount_; }

	/// <summary>
	/// Set the Debug Packet Count
	/// </summary>
	/// <param name="count">Number of packets to request</param>
	void SetDebugPacketCount(uint16_t count);

	/// <summary>
	/// Get the timestamp of the request
	/// </summary>
	/// <returns>DTC_EventWindowTag of reqeust</returns>
	DTC_EventWindowTag GetEventWindowTag() const { return event_tag_; }

	/// <summary>
	/// Convert a DTC_DataRequestPacket to DTC_DataPacket in "owner" mode
	/// </summary>
	/// <returns>DTC_DataPacket with DTC_DataRequestPacket contents set</returns>
	DTC_DataPacket ConvertToDataPacket() const override;
	/// <summary>
	/// Convert the DTC_DataRequestPacket to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DTC_DataRequestPacket</returns>
	std::string toJSON() override;
	/// <summary>
	/// Converts the DTC_DataRequestPacket to "packet format" representation (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DTC_DataRequestPacket</returns>
	std::string toPacketFormat() override;

private:
	DTC_EventWindowTag event_tag_;
	bool debug_;
	uint16_t debugPacketCount_;
	DTC_DebugType type_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataRequestPacket_h
