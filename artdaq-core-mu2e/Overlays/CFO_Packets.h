#ifndef CFO_PACKETS_H
#define CFO_PACKETS_H

#include <bitset>
#include <cstdint>  // uint8_t, uint16_t
#include <vector>
#include <cassert>

#include "DTC_Types.h"

using namespace DTCLib;

namespace CFOLib {

/// <summary>
/// Defined Packet Types for the CFO DMA Protocol
/// </summary>
enum CFO_PacketType : uint8_t
{
	CFO_PacketType_DataHeader = 5,
	CFO_PacketType_Invalid = 0x10,
};


/// <summary>
/// The CFO_DataPacket class represents the 16 bytes of raw data for all CFO packets.
/// The class works in two modes: "overlay" mode, where the data is in a fixed location in memory and modification is
/// restricted, and "owner" mode, where the DataPacket is a concrete instance.
/// </summary>
class CFO_DataPacket
{
public:
	/// <summary>
	/// Construct a CFO_DataPacket in owner mode
	/// </summary>
	CFO_DataPacket();

	/// <summary>
	/// Construct a CFO_DataPacket using a pointer to data. Flag will be set that the packet is read-only.
	/// </summary>
	/// <param name="data">Pointer to data</param>
	explicit CFO_DataPacket(const void* data)
		: dataPtr_(static_cast<const uint8_t*>(data)), dataSize_(16), memPacket_(true) {}

	/// <summary>
	/// Creates a copy of the CFO_DataPacket. Mode is preserved, if the existing DataPacket was in "owner" mode, a deep
	/// copy is made, otherwise the reference to the read-only memory will be copied.
	/// </summary>
	/// <param name="in">Input CFO_DataPacket</param>
	CFO_DataPacket(const CFO_DataPacket& in);
	/// <summary>
	/// Default move constructor
	/// </summary>
	/// <param name="in">CFO_DataPacket rvalue</param>
	CFO_DataPacket(CFO_DataPacket&& in) = default;

	virtual ~CFO_DataPacket();

	/// <summary>
	/// Default copy-assignment operator
	/// </summary>
	/// <param name="in">CFO_DataPacket lvalue</param>
	/// <returns>CFO_DataPacket reference</returns>
	CFO_DataPacket& operator=(const CFO_DataPacket& in) = default;
	/// <summary>
	/// Default move-assignment operator
	/// </summary>
	/// <param name="in">CFO_DataPacket rvalue</param>
	/// <returns>CFO_DataPacket reference</returns>
	CFO_DataPacket& operator=(CFO_DataPacket&& in) = default;

	/// <summary>
	/// Set the given byte of the DataPacket.
	/// No-op if the DataPacket is in overlay mode
	/// </summary>
	/// <param name="index">Index of the byte to change</param>
	/// <param name="data">Value of the byte</param>
	void SetByte(uint16_t index, uint8_t data);
	/// <summary>
	/// Gets the current value of the given byte
	/// </summary>
	/// <param name="index">Index of the byte</param>
	/// <returns>Value of the byte</returns>
	uint8_t GetByte(uint16_t index) const;
	/// <summary>
	/// Creates a JSON representation of the CFO_DataPacket
	/// </summary>
	/// <returns>JSON-formatted string representation of the CFO_DataPacket</returns>
	std::string toJSON() const;
	/// <summary>
	/// Create a "packet format" representation of the CFO_DataPacket. See "CFO Hardware User's Guide" for "packet format"
	/// representation.
	/// </summary>
	/// <returns>"packet format" string representation of the CFO_DataPacket</returns>
	std::string toPacketFormat() const;
	/// <summary>
	/// Resize a CFO_DataPacket in "owner" mode. New size must be larger than current.
	/// </summary>
	/// <param name="dmaSize">Size in bytes of the new packet</param>
	/// <returns>If the resize operation was successful</returns>
	bool Resize(const uint16_t dmaSize);

	/// <summary>
	/// Get the current size, in bytes, of the CFO_DataPacket (default: 16)
	/// </summary>
	/// <returns></returns>
	uint16_t GetSize() const { return dataSize_; }

	/// <summary>
	/// Determine whether the DataPacket is in owner mode or overlay mode
	/// </summary>
	/// <returns>True if the DataPacket is in overlay mode</returns>
	bool IsMemoryPacket() const { return memPacket_; }

	/// <summary>
	/// Add a CFO_DataPacket's data to this DataPacket. DataPacket must be large enough to accomodate data and in "owner"
	/// mode.
	/// </summary>
	/// <param name="other">Packet to add</param>
	/// <param name="offset">Where to copy packet in this DataPacket</param>
	/// <returns>True if successful</returns>
	bool CramIn(CFO_DataPacket& other, int offset)
	{
		if (other.dataSize_ + offset <= dataSize_)
		{
			memcpy(const_cast<uint8_t*>(dataPtr_) + offset, other.dataPtr_, other.dataSize_);
			return true;
		}
		return false;
	}

	/// <summary>
	/// Gets the pointer to the data
	/// </summary>
	/// <returns>Pointer to CFO_DataPacket data. Use GetSize() to determine the valid range of this pointer</returns>
	const uint8_t* GetData() const { return dataPtr_; }

	/// <summary>
	/// Comparison operator. Returns this.Equals(other)
	/// </summary>
	/// <param name="other">DataPacket to compare</param>
	/// <returns>this.Equals(other)</returns>
	bool operator==(const CFO_DataPacket& other) const { return Equals(other); }

	/// <summary>
	/// Comparison operator. Returns !this.Equals(other)
	/// </summary>
	/// <param name="other">DataPacket to compare</param>
	/// <returns>!this.Equals(other)</returns>
	bool operator!=(const CFO_DataPacket& other) const { return !Equals(other); }

	/// <summary>
	/// Compare the contents of two DataPackets. Ignores the first two bytes as they may differ (reserved words in most
	/// DMA packets).
	/// </summary>
	/// <param name="other">Data packet to compare</param>
	/// <returns>Whether the two DataPackets contents are equal</returns>
	bool Equals(const CFO_DataPacket& other) const;

	/// <summary>
	/// Serialize a CFO_DataPacket to the given ostream
	/// </summary>
	/// <param name="s">Stream to write to</param>
	/// <param name="p">DataPacket to stream, in binary</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& s, CFO_DataPacket& p)
	{
		return s.write(reinterpret_cast<const char*>(p.dataPtr_), p.dataSize_);
	}

	uint64_t event_tag_low : 32;
	uint64_t event_tag_high : 16;
	uint64_t event_mode : 40;
private:
	const uint8_t* dataPtr_;
	uint16_t dataSize_;
	bool memPacket_;
	std::vector<uint8_t> vals_;
};

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


struct CFO_EventHeader
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

	CFO_EventHeader()
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

	std::string toJson() const;
};

class CFO_Event
{
public:
	/// <summary>
	/// Construct a CFO_Event in "overlay" mode using the given DMA buffer pointer. Flag will be set that the packet
	/// is read-only.
	/// </summary>
	/// <param name="data">Pointer data</param>
	explicit CFO_Event(const void* data);

	explicit CFO_Event(size_t data_size);

	CFO_Event()
		: //header_(), sub_events_(), 
		buffer_ptr_(nullptr) {}

	static const int MAX_DMA_SIZE = 0x8000;	// 32k

	void SetupEvent();
	size_t GetEventByteCount() const { return 16; } //header_.inclusive_event_byte_count; }
	DTC_EventWindowTag GetEventWindowTag() const;
	void SetEventWindowTag(DTC_EventWindowTag const& tag);
	void SetEventMode(DTC_EventMode const& mode);
	const void* GetRawBufferPointer() const { return buffer_ptr_; }

	// std::vector<DTC_SubEvent> const& GetSubEvents() const
	// {
	// 	return sub_events_;
	// }
	// size_t GetSubEventCount() const { return sub_events_.size(); }

	// size_t GetSubEventCount(DTC_Subsystem subsys) const
	// {
	// 	size_t count = 0;
	// 	for (size_t ii = 0; ii < sub_events_.size(); ++ii)
	// 	{
	// 		if (sub_events_[ii].GetSubsystem() == subsys) ++count;
	// 	}
	// 	return count;
	// }

	// size_t GetBlockCount(DTC_Subsystem subsys) const
	// {
	// 	size_t count = 0;
	// 	for (size_t ii = 0; ii < sub_events_.size(); ++ii)
	// 	{
	// 		if (sub_events_[ii].GetSubsystem() == subsys)
	// 		{
	// 			count += sub_events_[ii].GetDataBlockCount();
	// 		}
	// 	}
	// 	return count;
	// }

	// DTC_SubEvent* GetSubEvent(size_t idx)
	// {
	// 	if (idx >= sub_events_.size()) throw std::out_of_range("Index " + std::to_string(idx) + " is out of range (max: " + std::to_string(sub_events_.size() - 1) + ")");
	// 	return &sub_events_[idx];
	// }

	// void AddSubEvent(DTC_SubEvent subEvt)
	// {
	// 	sub_events_.push_back(subEvt);
	// 	header_.num_dtcs++;
	// 	UpdateHeader();
	// }
	// DTC_SubEvent* GetSubEventByDTCID(uint8_t dtc, DTC_Subsystem subsys)
	// {
	// 	auto dtcid = (dtc & 0xF) + ((static_cast<uint8_t>(subsys) & 0x7) << 4);
	// 	for (size_t ii = 0; ii < sub_events_.size(); ++ii)
	// 	{
	// 		if (sub_events_[ii].GetDTCID() == dtcid) return &sub_events_[ii];
	// 	}
	// 	return nullptr;
	// }

	// std::vector<DTC_SubEvent> GetSubsystemData(DTC_Subsystem subsys) const {
	// 	std::vector<DTC_SubEvent> output;

	// 	for(auto& subevt : sub_events_) {
	// 		if(subevt.GetSubsystem() == subsys) {
	// 			output.push_back(subevt);
	// 		}
	// 	}

	// 	return output;
	// }

	// DTC_EventHeader* GetHeader() { return &header_; }

	// void UpdateHeader();
	void WriteEvent(std::ostream& output, bool includeDMAWriteSize = true);

private:
	std::shared_ptr<std::vector<uint8_t>> allocBytes{nullptr};  ///< Used if the block owns its memory
	CFO_EventHeader header_;
	// std::vector<DTC_SubEvent> sub_events_;
	const void* buffer_ptr_;
};

}  // namespace CFOLib

#endif  // CFO_PACKETS_H
