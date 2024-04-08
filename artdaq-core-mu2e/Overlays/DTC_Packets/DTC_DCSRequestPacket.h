#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSRequestPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSRequestPacket_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DMAPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_DCSOperationType.h"

#include <cstdint>
#include <string>
#include <utility> // std::pair
#include <vector>

namespace DTCLib {

/// <summary>
/// Representation of a DCS Request Packet
/// </summary>
class DTC_DCSRequestPacket : public DTC_DMAPacket
{
public:
	/// <summary>
	/// Default Constructor, zeroes out header fields
	/// </summary>
	DTC_DCSRequestPacket();
	/// <summary>
	/// DCSRequestPacket constructor, using given link and roc
	/// </summary>
	/// <param name="link">Link ID for packet</param>
	DTC_DCSRequestPacket(DTC_Link_ID link);
	/// <summary>
	/// Create a DTC_DCSRequestPacket instance with the given fields filled in
	/// </summary>
	/// <param name="link">Link ID for packet</param>
	/// <param name="type">OpCode of packet</param>
	/// <param name="requestAck">Whether to request acknowledement of this operation</param>
	/// <param name="incrementAddress">Whether to increment the address pointer for block reads/writes</param>
	/// <param name="address">Address of ROC register</param>
	/// <param name="data">Data/wordCount for operation</param>
	/// <param name="address2">Address of ROC register</param>
	/// <param name="data2">Data/wordCount for operation</param>
	DTC_DCSRequestPacket(DTC_Link_ID link, DTC_DCSOperationType type, bool requestAck, bool incrementAddress, uint16_t address,
						 uint16_t data = 0x0, uint16_t address2 = 0x0, uint16_t data2 = 0x0);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="in">DTC_DCSRequestPacket to copy</param>
	DTC_DCSRequestPacket(const DTC_DCSRequestPacket& in) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="in">DTC_DCSRequestPacket rvalue</param>
	DTC_DCSRequestPacket(DTC_DCSRequestPacket&& in) = default;
	/// <summary>
	/// Construct a DTC_DCSRequestPacket using the data in the given DataPacket
	/// </summary>
	/// <param name="in">DataPacket to parse</param>
	explicit DTC_DCSRequestPacket(const DTC_DataPacket in);

	/// <summary>
	/// Default Copy Assignment Operator
	/// </summary>
	/// <param name="in">DTC_DCSRequestPacket to copy</param>
	/// <returns>DTC_DCSRequestPacket Reference</returns>
	DTC_DCSRequestPacket& operator=(const DTC_DCSRequestPacket& in) = default;
	/// <summary>
	/// Default Move Assignment Operator
	/// </summary>
	/// <param name="in">DTC_DCSRequestPacket rvalue</param>
	/// <returns>DTC_DCSRequestPacket Reference</returns>
	DTC_DCSRequestPacket& operator=(DTC_DCSRequestPacket&& in) = default;

	virtual ~DTC_DCSRequestPacket() = default;

	/// <summary>
	/// Gets the opcode of the DCS Request Packet
	/// </summary>
	/// <returns>Current Opcode of the DCS Request Packet</returns>
	DTC_DCSOperationType GetType() const { return type_; }

	/// <summary>
	/// Read the double operation bit from the DCS Request packet
	/// </summary>
	/// <returns>Whether the Double operation bit is set</returns>
	bool IsDoubleOp() const { return (type_ & 0x4) != 0; }

	/// <summary>
	/// Read the request acknowledgment bit from the DCS Request Packet
	/// </summary>
	/// <returns>Whether the request acknowledgment bit is set</returns>
	bool RequestsAck() const { return requestAck_; }

	/// <summary>
	/// Read the increment address bit from the DCS Request Packet
	/// </summary>
	/// <returns>Whether the increment address bit is set</returns>
	bool IncrementsAddress() const { return incrementAddress_; }

	/// <summary>
	/// Get the request from the DCS Request Packet
	/// </summary>
	/// <param name="secondOp">Whether to read the second request</param>
	/// <returns>Pair of address, data from the given request</returns>
	std::pair<uint16_t, uint16_t> GetRequest(bool secondOp = false)
	{
		if (!secondOp) return std::make_pair(address1_, data1_);
		return std::make_pair(address2_, data2_);
	}

	/// <summary>
	/// Add a second request to the DCS Request Packet
	/// </summary>
	/// <param name="address">Address of the second request</param>
	/// <param name="data">Data for the second request</param>
	void AddRequest(uint16_t address, uint16_t data = 0x0);
	/// <summary>
	/// Set the block write data
	/// </summary>
	/// <param name="data">Vector of 16-bit words to write to the ROC</param>
	void SetBlockWriteData(std::vector<uint16_t> data)
	{
		blockWriteData_ = data;
		UpdatePacketAndWordCounts();
	}
	/// <summary>
	/// Update the packetCount and data_ (Block word count) words in the DCS request packet, based on the type of operation and blockWriteData
	/// </summary>
	void UpdatePacketAndWordCounts();

	/// <summary>
	/// Sets the opcode of the DCS Request Packet
	/// </summary>
	/// <param name="type">Opcode to set</param>
	/// <param name="reqAck">Whether to request acknowledgment of this operation</param>
	/// <param name="incAddress">Whether to increment the address pointer for block reads/writes</param>
	void SetType(DTC_DCSOperationType type, bool reqAck, bool incAddress)
	{
		requestAck_ = reqAck;
		incrementAddress_ = incAddress;
		type_ = type;
	}

	/// <summary>
	/// Convert a DTC_DCSRequestPacket to DTC_DataPacket in "owner" mode
	/// </summary>
	/// <returns>DTC_DataPacket with DCS Request Packet contents set</returns>
	DTC_DataPacket ConvertToDataPacket() const override;
	/// <summary>
	/// Convert the DCS Request Packet to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DCS Request packet</returns>
	std::string toJSON() override;
	/// <summary>
	/// Converts the DCS Request Packet to "packet format" representation (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DCS Request packet</returns>
	std::string toPacketFormat() override;

private:
	DTC_DCSOperationType type_;
	bool requestAck_;
	bool incrementAddress_;
	uint16_t packetCount_;
	uint16_t address1_;
	uint16_t data1_;     ///< Also, blockWriteData0_
	uint16_t address2_;  ///< Also, blockWriteData1_
	uint16_t data2_;     ///< Also, blockWriteData2_
	std::vector<uint16_t> blockWriteData_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSRequestPacket_h
