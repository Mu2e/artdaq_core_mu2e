#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DMAPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DMAPacket_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_PacketType.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"

#include <cstdint>
#include <ostream>
#include <string>

namespace DTCLib {

/// <summary>
/// Header information common to all DTC Packets (except Data Packets)
/// </summary>
class DTC_DMAPacket
{
protected:
	uint16_t byteCount_;         ///< Byte count of current block
	bool valid_;                 ///< Whether the DTC believes the packet to be valid
	uint8_t subsystemID_;        ///< Subsystem ID (Data Header packet only)
	DTC_Link_ID linkID_;         ///< Link identifier of packet
	DTC_PacketType packetType_;  ///< Packet type
	uint8_t hopCount_;           ///< Hop count
public:
	/// <summary>
	/// DTC_DMAPacket default constructor. Fills in header fields with default (invalid) values.
	/// </summary>
	DTC_DMAPacket()
		: byteCount_(0), valid_(false), subsystemID_(0), linkID_(DTC_Link_Unused), packetType_(DTC_PacketType_Invalid), hopCount_(0) {}

	/// <summary>
	/// Create a DTC_DMAPacket with the given parameters
	/// </summary>
	/// <param name="type">Packet Type</param>
	/// <param name="link">Link ID</param>
	/// <param name="byteCount">Block byte count. Default is one packet, 16 bytes</param>
	/// <param name="valid">Valid flag for packet, default true</param>
	/// <param name="subsystemID">Subsystem ID for packet</param>
	/// <param name="hopCount">Hop count for packet, default 0</param>
	DTC_DMAPacket(DTC_PacketType type, DTC_Link_ID link, uint16_t byteCount = 16, bool valid = true, uint8_t subsystemID = 0, uint8_t hopCount = 0);

	/// <summary>
	/// Construct a DTC_DMAPacket using the data in the given DataPacket
	/// </summary>
	/// <param name="in">DTC_DataPacket to interpret</param>
	explicit DTC_DMAPacket(const DTC_DataPacket in);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="in">DTC_DMAPacket to copy</param>
	DTC_DMAPacket(const DTC_DMAPacket& in) = default;
	/// <summary>
	/// Default move Constructor
	/// </summary>
	/// <param name="in">DTC_DMAPacket rvalue</param>
	DTC_DMAPacket(DTC_DMAPacket&& in) = default;

	virtual ~DTC_DMAPacket() = default;

	/// <summary>
	/// Default Copy Assignment Operator
	/// </summary>
	/// <param name="in">DTC_DMAPacket to copy</param>
	/// <returns>DTC_DMAPacket reference</returns>
	DTC_DMAPacket& operator=(const DTC_DMAPacket& in) = default;
	/// <summary>
	/// Default Move Assignment Operator
	/// </summary>
	/// <param name="in">DTC_DMAPacket rvalue</param>
	/// <returns>DTC_DMAPacket reference</returns>
	DTC_DMAPacket& operator=(DTC_DMAPacket&& in) = default;

	/// <summary>
	/// Convert a DTC_DMAPacket to DTC_DataPacket in "owner" mode
	/// </summary>
	/// <returns>DTC_DataPacket with DMA Header bytes set</returns>
	virtual DTC_DataPacket ConvertToDataPacket() const;

	/// <summary>
	/// Packet Type accessor
	/// </summary>
	/// <returns>Packet Type of DMA Packet</returns>
	DTC_PacketType GetPacketType() const { return packetType_; }

	/// <summary>
	/// Gets the DMA Header in JSON
	/// </summary>
	/// <returns>JSON-formatted string representation of DMA Header information</returns>
	std::string headerJSON() const;
	/// <summary>
	/// Gets the DMA header in "packet format" (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DMA header information</returns>
	std::string headerPacketFormat() const;

	/// <summary>
	/// Returns if the DTC thinks the packet is valid
	/// </summary>
	/// <returns>The valid bit of the packet</returns>
	bool isValid() const { return valid_; }

	/// <summary>
	/// Gets the block byte count
	/// </summary>
	/// <returns>Block byte count of DMA Header</returns>
	uint16_t GetByteCount() const { return byteCount_; }

	/// <summary>
	/// Gets the Link ID of the packet
	/// </summary>
	/// <returns>The Link ID of the packet</returns>
	DTC_Link_ID GetLinkID() const { return linkID_; }

	/// <summary>
	/// Gets the Hop Count of the packet
	///
	/// </summary>
	/// <returns>The Hop count of the packet</returns>
	uint8_t GetHopCount() const
	{
		return hopCount_;
	}
	/// <summary>
	/// Gets the Subsystem ID of the packet
	///
	/// This method should only be used if the packet is a DataHeader packet
	/// </summary>
	/// <returns>Subsystem ID stored in the packet</returns>
	uint8_t GetSubsystemID() const
	{
		return subsystemID_;
	}

	/// <summary>
	/// Converts the DMA Packet to "packet format" representation (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DMA packet</returns>
	virtual std::string toPacketFormat();
	/// <summary>
	/// Convert the DMA Packet to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DMA packet</returns>
	virtual std::string toJSON();

	/// <summary>
	/// Stream the JSON representation of the DTC_DMAPacket to the given stream
	/// </summary>
	/// <param name="stream">Stream to write JSON data to</param>
	/// <param name="packet">Packet to stream</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, DTC_DMAPacket& packet)
	{
		stream << packet.toJSON();
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DMAPacket_h
