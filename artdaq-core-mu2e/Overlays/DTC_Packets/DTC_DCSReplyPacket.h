#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSReplyPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSReplyPacket_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DMAPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_DCSOperationType.h"

#include <cstdint>
#include <utility> // std::pair
#include <vector>

namespace DTCLib {

/// <summary>
/// The DCS Reply Packet
/// </summary>
class DTC_DCSReplyPacket : public DTC_DMAPacket
{
public:
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="right">DTC_DCSReplyPacket to copy</param>
	DTC_DCSReplyPacket(const DTC_DCSReplyPacket& right) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="right">DTC_DCSReplyPacket to move</param>
	DTC_DCSReplyPacket(DTC_DCSReplyPacket&& right) = default;
	/// <summary>
	/// Construct a DTC_DCSReplyPacket from the given DTC_DataPacket
	/// </summary>
	/// <param name="in">DTC_DataPacket to overlay</param>
	explicit DTC_DCSReplyPacket(const DTC_DataPacket in);

	/// <summary>
	/// Get the value packet count field
	/// </summary>
	/// <returns>The number of packets in the block read</returns>
	uint8_t GetDTCErrorBits() const { return DTCErrorBits_; }

	/// <summary>
	/// Get the DCS Operation Type
	/// </summary>
	/// <returns>DTC_DCSOperationType enumeration value</returns>
	DTC_DCSOperationType GetType() const { return type_; }

	/// <summary>
	/// Read the double operation bit from the DCS reply packet
	/// </summary>
	/// <returns>Whether the double operation bit is set</returns>
	bool IsDoubleOperation() const { return doubleOp_; }

	/// <summary>
	/// Get the "request acknowledgment" bit from the DCS Reply packet
	/// </summary>
	/// <returns></returns>
	bool IsAckRequested() const { return requestAck_; }

	/// <summary>
	/// Check if the DCS Receive FIFO is empty
	/// </summary>
	/// <returns>Value of DCS Receive FIFO Empty flag</returns>
	bool DCSReceiveFIFOEmpty() const { return dcsReceiveFIFOEmpty_; }

	/// <summary>
	/// Get the corrupt flag from the status word
	/// </summary>
	/// <returns>Whether the corrupt bit is set</returns>
	bool ROCIsCorrupt() const { return corruptFlag_; }

	/// <summary>
	/// Get the value packet count field
	/// </summary>
	/// <returns>The number of packets in the block read</returns>
	uint16_t GetBlockPacketCount() const { return packetCount_; }

	/// <summary>
	/// Get the reply from the DCSReplyPacket
	/// </summary>
	/// <param name="secondOp">Whether to read the second operation</param>
	/// <returns>Pair of address, data from the reply packet</returns>
	std::pair<uint16_t, uint16_t> GetReply(bool secondOp = false)
	{
		if (!secondOp) return std::make_pair(address1_, data1_);
		return std::make_pair(address2_, data2_);
	}

	/// <summary>
	/// Get the block read data, if any
	/// </summary>
	/// <returns>Vector of 16-bit words</returns>
	const std::vector<uint16_t>& GetBlockReadData() const { return blockReadData_; }

	/// <summary>
	/// Convert a DTC_DCSReplyPacket to DTC_DataPacket in "owner" mode
	/// </summary>
	/// <returns>DTC_DataPacket with DTC_DCSReplyPacket contents set</returns>
	DTC_DataPacket ConvertToDataPacket() const override;
	/// <summary>
	/// Convert the DTC_DCSReplyPacket to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DTC_DCSReplyPacket</returns>
	std::string toJSON() override;
	/// <summary>
	/// Converts the DTC_DCSReplyPacket to "packet format" representation (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DTC_DCSReplyPacket</returns>
	std::string toPacketFormat() override;

private:
	uint8_t DTCErrorBits_;
	DTC_DCSOperationType type_;
	bool doubleOp_;
	bool requestAck_;
	bool dcsReceiveFIFOEmpty_;
	bool corruptFlag_;
	uint16_t packetCount_;
	uint16_t address1_;
	uint16_t data1_;
	uint16_t address2_;
	uint16_t data2_;
	std::vector<uint16_t> blockReadData_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSReplyPacket_h
