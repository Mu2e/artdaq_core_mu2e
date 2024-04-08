#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataHeaderPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataHeaderPacket_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DMAPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventWindowTag.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Subsystem.h"

#include <cstdint>
#include <string>

namespace DTCLib {

/// <summary>
/// The DTC Data Header Packet (A Data Header and its associated Data Packets forms a Data Block)
/// </summary>
class DTC_DataHeaderPacket : public DTC_DMAPacket
{
public:
	/// <summary>
	/// Construct a DTC_DataHeaderPacket
	/// </summary>
	/// <param name="link">Link from which packet came</param>
	/// <param name="packetCount">Number of DTC_DataPackets in Data Block</param>
	/// <param name="status">Status of Data Block</param>
	/// <param name="dtcid">DTC ID from which packet came</param>
	/// <param name="subsystemid">Subsystem ID from which packet came</param>
	/// <param name="packetVersion">Version of data format</param>
	/// <param name="event_tag">Timestamp of Data Packet (Default: DTC_Timetstamp())</param>
	/// <param name="evbMode">EVB Mode byte (Default: 0)</param>
	DTC_DataHeaderPacket(DTC_Link_ID link, uint16_t packetCount, uint8_t status, uint8_t dtcid, DTC_Subsystem subsystemid,
						 uint8_t packetVersion, DTC_EventWindowTag event_tag = DTC_EventWindowTag(), uint8_t evbMode = 0);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="right">DTC_DataHeaderPacket to copy</param>
	DTC_DataHeaderPacket(const DTC_DataHeaderPacket& right) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="right">DTC_DataHeaderPacket to move</param>
	DTC_DataHeaderPacket(DTC_DataHeaderPacket&& right) = default;
	/// <summary>
	/// Construct a DTC_DataHeaderPacket from the given DTC_DataPacket
	/// </summary>
	/// <param name="in">DTC_DataPacket to overlay</param>
	explicit DTC_DataHeaderPacket(const DTC_DataPacket in);

	/// <summary>
	/// Convert a DTC_DataHeaderPacket to DTC_DataPacket in "owner" mode
	/// </summary>
	/// <returns>DTC_DataPacket with DTC_DataHeaderPacket contents set</returns>
	DTC_DataPacket ConvertToDataPacket() const override;

	/// <summary>
	/// Get the EVB Mode word from the Data Header Packet
	/// </summary>
	/// <returns>EVB Mode of Data Block</returns>
	uint8_t GetEVBMode() const { return evbMode_; }

	/// <summary>
	/// Get the Subsystem ID of the Data Block
	/// </summary>
	/// <returns>DTC_Subsystem enumeration value</returns>
	DTC_Subsystem GetSubsystem() const { return static_cast<DTC_Subsystem>(GetSubsystemID()); }

	/// <summary>
	/// Get the DTC ID of the Data Block
	/// </summary>
	/// <returns>DTC ID of Data Block</returns>
	uint8_t GetID() const { return dtcId_; }

	/// <summary>
	/// Get the number of Data Packets in the Data block
	/// </summary>
	/// <returns>The number of packets in the Data Block</returns>
	uint16_t GetPacketCount() const { return packetCount_; }

	/// <summary>
	/// Get the Data Packet Version identifier from the Data Header
	/// </summary>
	/// <returns>Version number of Data Packets</returns>
	uint8_t GetVersion() const { return dataPacketVersion_; }

	/// <summary>
	/// Get the Timestamp of the Data Block
	/// </summary>
	/// <returns>timestamp of Data Block</returns>
	DTC_EventWindowTag GetEventWindowTag() const { return event_tag_; }

	/// <summary>
	/// Get the Data Status of the Data Block
	/// </summary>
	/// <returns>DTC_DataStatus enumeration value</returns>
	uint8_t GetStatus() const { return status_; }

	/// <summary>
	/// Convert the DTC_DataHeaderPacket to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DTC_DataHeaderPacket</returns>
	std::string toJSON() override;
	/// <summary>
	/// Converts the DTC_DataHeaderPacket to "packet format" representation (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DTC_DataHeaderPacket</returns>
	std::string toPacketFormat() override;

	/// <summary>
	/// Determine if two Data Header packets are equal (Evaluates DataPacket == DataPacket, see DTC_DataPacket::Equals)
	/// </summary>
	/// <param name="other">DataHeaderPacket to compare</param>
	/// <returns>True if Data Header packet contents are equal</returns>
	bool operator==(const DTC_DataHeaderPacket& other) const { return Equals(other); }

	/// <summary>
	/// Determine if two Data Header packets are not equal (Returns !(dhp == dhp))
	/// </summary>
	/// <param name="other">DataHeaderPacket to compare</param>
	/// <returns>True if Data Header packet contents are not equal</returns>
	bool operator!=(const DTC_DataHeaderPacket& other) const { return !Equals(other); }

	/// <summary>
	/// Determine if two Data Header packets are equal (Evaluates DataPacket == DataPacket, see DTC_DataPacket::Equals)
	/// </summary>
	/// <param name="other">DataHeaderPacket to compare</param>
	/// <returns>True if Data Header packet contents are equal</returns>
	bool Equals(const DTC_DataHeaderPacket& other) const;

private:
	uint16_t packetCount_;
	DTC_EventWindowTag event_tag_;
	uint8_t status_;
	uint8_t dataPacketVersion_;
	uint8_t dtcId_;
	uint8_t evbMode_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataHeaderPacket_h
