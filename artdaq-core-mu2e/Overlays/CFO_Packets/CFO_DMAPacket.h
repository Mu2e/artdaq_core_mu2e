#ifndef artdaq_core_mu2e_Overlays_CFO_Packets_CFO_DMAPacket_h
#define artdaq_core_mu2e_Overlays_CFO_Packets_CFO_DMAPacket_h

#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_DataPacket.h"
#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_PacketType.h"

#include <cstdint>
#include <ostream>
#include <string>

namespace CFOLib {

/// <summary>
/// Header information common to all CFO Packets (except Data Packets)
/// </summary>
class CFO_DMAPacket
{
protected:
	uint16_t byteCount_;         ///< Byte count of current block
	bool valid_;                 ///< Whether the CFO believes the packet to be valid
	// uint8_t subsystemID_;        ///< Subsystem ID (Data Header packet only)
	// CFO_Link_ID linkID_;         ///< Link identifier of packet
	CFO_PacketType packetType_;  ///< Packet type
	// uint8_t hopCount_;           ///< Hop count
public:
	/// <summary>
	/// CFO_DMAPacket default constructor. Fills in header fields with default (invalid) values.
	/// </summary>
	CFO_DMAPacket()
		: byteCount_(0), valid_(false), 
		// subsystemID_(0), 
		// linkID_(DTC_Link_Unused), 
		packetType_(CFO_PacketType_Invalid) 
		// hopCount_(0) 
		{}

	/// <summary>
	/// Create a CFO_DMAPacket with the given parameters
	/// </summary>
	/// <param name="type">Packet Type</param>
	/// <param name="link">Link ID</param>
	/// <param name="byteCount">Block byte count. Default is one packet, 16 bytes</param>
	/// <param name="valid">Valid flag for packet, default true</param>
	/// <param name="subsystemID">Subsystem ID for packet</param>
	/// <param name="hopCount">Hop count for packet, default 0</param>
	CFO_DMAPacket(CFO_PacketType type, 
	// DTC_Link_ID link, 
		uint16_t byteCount = 16, bool valid = true 
		// uint8_t subsystemID = 0
			//, uint8_t hopCount = 0
			);

	/// <summary>
	/// Construct a CFO_DMAPacket using the data in the given DataPacket
	/// </summary>
	/// <param name="in">CFO_DataPacket to interpret</param>
	explicit CFO_DMAPacket(const CFO_DataPacket in);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="in">CFO_DMAPacket to copy</param>
	CFO_DMAPacket(const CFO_DMAPacket& in) = default;
	/// <summary>
	/// Default move Constructor
	/// </summary>
	/// <param name="in">CFO_DMAPacket rvalue</param>
	CFO_DMAPacket(CFO_DMAPacket&& in) = default;

	virtual ~CFO_DMAPacket() = default;

	/// <summary>
	/// Default Copy Assignment Operator
	/// </summary>
	/// <param name="in">CFO_DMAPacket to copy</param>
	/// <returns>CFO_DMAPacket reference</returns>
	CFO_DMAPacket& operator=(const CFO_DMAPacket& in) = default;
	/// <summary>
	/// Default Move Assignment Operator
	/// </summary>
	/// <param name="in">CFO_DMAPacket rvalue</param>
	/// <returns>CFO_DMAPacket reference</returns>
	CFO_DMAPacket& operator=(CFO_DMAPacket&& in) = default;

	/// <summary>
	/// Convert a CFO_DMAPacket to CFO_DataPacket in "owner" mode
	/// </summary>
	/// <returns>CFO_DataPacket with DMA Header bytes set</returns>
	virtual CFO_DataPacket ConvertToDataPacket() const;

	/// <summary>
	/// Packet Type accessor
	/// </summary>
	/// <returns>Packet Type of DMA Packet</returns>
	CFO_PacketType GetPacketType() const { return packetType_; }

	/// <summary>
	/// Gets the DMA Header in JSON
	/// </summary>
	/// <returns>JSON-formatted string representation of DMA Header information</returns>
	std::string headerJSON() const;
	/// <summary>
	/// Gets the DMA header in "packet format" (See CFO_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DMA header information</returns>
	std::string headerPacketFormat() const;

	/// <summary>
	/// Returns if the CFO thinks the packet is valid
	/// </summary>
	/// <returns>The valid bit of the packet</returns>
	bool isValid() const { return valid_; }

	/// <summary>
	/// Gets the block byte count
	/// </summary>
	/// <returns>Block byte count of DMA Header</returns>
	uint16_t GetByteCount() const { return byteCount_; }

	// /// <summary>
	// /// Gets the Link ID of the packet
	// /// </summary>
	// /// <returns>The Link ID of the packet</returns>
	// CFO_Link_ID GetLinkID() const { return linkID_; }

	// /// <summary>
	// /// Gets the Hop Count of the packet
	// ///
	// /// </summary>
	// /// <returns>The Hop count of the packet</returns>
	// uint8_t GetHopCount() const
	// {
	// 	return hopCount_;
	// }
	/// <summary>
	/// Gets the Subsystem ID of the packet
	///
	/// This method should only be used if the packet is a DataHeader packet
	/// </summary>
	/// <returns>Subsystem ID stored in the packet</returns>
	// uint8_t GetSubsystemID() const
	// {
	// 	return subsystemID_;
	// }

	/// <summary>
	/// Converts the DMA Packet to "packet format" representation (See CFO_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DMA packet</returns>
	virtual std::string toPacketFormat();
	/// <summary>
	/// Convert the DMA Packet to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DMA packet</returns>
	virtual std::string toJSON();

	/// <summary>
	/// Stream the JSON representation of the CFO_DMAPacket to the given stream
	/// </summary>
	/// <param name="stream">Stream to write JSON data to</param>
	/// <param name="packet">Packet to stream</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, CFO_DMAPacket& packet)
	{
		stream << packet.toJSON();
		return stream;
	}
};

}  // namespace CFOLib

#endif  // artdaq_core_mu2e_Overlays_CFO_Packets_CFO_DMAPacket_h
