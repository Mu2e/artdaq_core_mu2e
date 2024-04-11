#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DCSReplyPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Exceptions.h"

#include "TRACE/tracemf.h"

#include <iomanip>

// ~~~ Packet Types ~~~ (**** can be sent s2c, --- can be sent by CFO, @@@ can be sent by ROC)
// 0 DCS Request   ****
// 1 Heartbeat (broadcast) ---
// 2 Data Request ****
// 3 Prefetch Request ****
// 4 DCS Reply @@@
// 5 Data Header @@@
// 6 Data Payload @@@
// 7 DCS Additional Block Write Payload  ****
// 8 DCS Reply Additional Block Read Payload @@@
// 9-15 Reserved
// ~~~~ DCS Packet Request or Software Data Request Packet ~~~~
// [15:0]  DMA Byte Count High DMA Byte Count Low
// [31:16] Valid [15] DTC Errors [14:11] ROC Link ID [10:8] Packet Type (0x0) [7:4] Hop Count [3:0]   |||| Common for all packets!
// [47:32] Block Op Additional Packet Count [15:6] ROC Status [5:4] Op Code [3:0]		 |||| Low bytes of EWT in Data Request and Prefetch Packets
// [63:48] Op1 Address [15:0]
// [15:0]  Op1 Write Data [15:0] or Block Op Word Count [15:0]
// [31:16] Op2 Address [15:0] or Block Write Data0 [15:0]
// [47:32] Op2 Write Data [15:0] or Block Write Data1 [15:0]
// [63:48] Reserved or Block Write Data2 [15:0]

// The DTC Error 4-bit field is defined as follows (these are flags identified at the DTC FPGA):
// Bit Position	Definition
// 0	SERDES PLL associated with the ROC has lost lock
// 1	SERDES associated with the ROC has lost clock-data-recovery lock
// 2	Invalid packet has been received from ROC
// 3	Error in DTC handling of this ROC’s DCS requests has occurred

// The ROC Status 2-bit field is defined as follows
// 0	“DCS Request FIFO Empty” flag indicates that there are more data requests queued when value is low.
// 1	“I am corrupt” flag indicates the ROC has lost DCS data or the ability to conduct DCS readout has been compromised.

DTCLib::DTC_DCSReplyPacket::DTC_DCSReplyPacket(DTC_DataPacket in)
	: DTC_DMAPacket(in)
{
	TRACE(20, "DTC_DCSReplyPacket::DTC_DCSReplyPacket Before packetType test");
	if (packetType_ != DTC_PacketType_DCSReply)
	{
		auto ex = DTC_WrongPacketTypeException(DTC_PacketType_DCSReply, packetType_);
		TLOG(TLVL_ERROR) << ex.what();
		throw ex;
	}
	DTCErrorBits_ = (in.GetData()[3] >> 3)  & 0xF; //[31:16] Valid [15] DTC Errors [14:11] ROC Link ID [10:8] 

	uint8_t tmpType = in.GetData()[4] & 0xF;
	if(static_cast<DTC_DCSOperationType>(tmpType) != DTC_DCSOperationType_InvalidS2C && 
		static_cast<DTC_DCSOperationType>(tmpType) != DTC_DCSOperationType_Timeout)
		tmpType &= 0x3; //if known type, allow complex types in 4-bit nibble; otherwise, mask off	
	type_ = static_cast<DTC_DCSOperationType>(tmpType);
	doubleOp_ = (in.GetData()[4] & 0x4) == 0x4;
	requestAck_ = (in.GetData()[4] & 0x8) == 0x8;
	
	dcsReceiveFIFOEmpty_ = (in.GetData()[4] & 0x10) == 0x10;
	corruptFlag_ = (in.GetData()[4] & 0x20) == 0x20;

	packetCount_ = (in.GetData()[4] >> 6) + (in.GetData()[5] << 2);
	address1_ = in.GetData()[6] + (in.GetData()[7] << 8);
	data1_ = in.GetData()[8] + (in.GetData()[9] << 8);

	if (type_ == DTC_DCSOperationType_BlockRead)
	{
		address2_ = 0;
		data2_ = 0;
		if (data1_ > 0)
		{
			blockReadData_.push_back(in.GetData()[10] + (in.GetData()[11] << 8));
		}
		if (data1_ > 1)
		{
			blockReadData_.push_back(in.GetData()[12] + (in.GetData()[13] << 8));
		}
		if (data1_ > 2)
		{
			blockReadData_.push_back(in.GetData()[14] + (in.GetData()[15] << 8));
		}

		if (in.GetSize() > 16)
		{
			size_t wordCounter = 16;
			while (wordCounter < in.GetSize())
			{
				blockReadData_.push_back(in.GetData()[wordCounter] + (in.GetData()[wordCounter + 1] << 8));
				wordCounter += 2;
			}
		}
	}
	else
	{
		address2_ = in.GetData()[10] + (in.GetData()[11] << 8);
		data2_ = in.GetData()[12] + (in.GetData()[13] << 8);
	}
}

std::string DTCLib::DTC_DCSReplyPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"DCSReplyPacket\": {\n";
	ss << headerJSON();
	ss << ",\n\"Operation Type\":" << DTC_DCSOperationTypeConverter(type_);
	ss << ",\n\"Double Operation\":" << (doubleOp_ ? "\"true\"" : "\"false\"");
	ss << ",\n\"Request Acknowledgement\":" << (requestAck_ ? "\"true\"" : "\"false\"");
	ss << ",\n\"DCS Request FIFO Empty\": " << (dcsReceiveFIFOEmpty_ ? "\"true\"" : "\"false\"");
	ss << ",\n\"Corrupt Flag\": " << (corruptFlag_ ? "\"true\"" : "\"false\"");
	ss << ",\n\"Address1\": " << static_cast<int>(address1_);
	if (type_ != DTC_DCSOperationType_BlockRead)
	{
		ss << ",\n\"Data1\": " << static_cast<int>(data1_);
		ss << ",\n\"Address2\": " << static_cast<int>(address2_);
		ss << ",\n\"Data2\": " << static_cast<int>(data2_);
	}
	else
	{
		ss << ",\n\"Block Word Count\": " << static_cast<int>(data1_);
		auto counter = 0;
		for (auto& word : blockReadData_)
		{
			ss << ",\n\"Block Read word " << counter << "\":" << static_cast<int>(word);
			counter++;
		}
	}
	ss << "\n}";
	return ss.str();
}

std::string DTCLib::DTC_DCSReplyPacket::toPacketFormat()
{
	std::stringstream ss;
	
	ss << headerPacketFormat() << std::hex << std::setfill('0');

	auto firstWord = (packetCount_ & 0x3FC) >> 2;
	auto secondWord = ((packetCount_ & 0x3) << 6) + (corruptFlag_ ? 0x20 : 0) + (dcsReceiveFIFOEmpty_ ? 0x10 : 0) +
		(requestAck_ ? 0x8 : 0) + (doubleOp_ ? 0x4 : 0) + static_cast<int>(type_);
	ss << std::setw(8) << firstWord << "\t" << secondWord << std::endl;

	ss << std::setw(8) << ((address1_ & 0xFF00) >> 8) << "\t" << (address1_ & 0xFF) << std::endl;
	ss << std::setw(8) << ((data1_ & 0xFF00) >> 8) << "\t" << (data1_ & 0xFF) << std::endl;
	if (type_ != DTC_DCSOperationType_BlockRead)
	{
		ss << std::setw(8) << ((address2_ & 0xFF00) >> 8) << "\t" << (address2_ & 0xFF) << std::endl;
		ss << std::setw(8) << ((data2_ & 0xFF00) >> 8) << "\t" << (data2_ & 0xFF) << std::endl;
		ss << "        \t        " << std::endl;
	}
	else
	{
		if (blockReadData_.size() > 0)
		{
			ss << std::setw(8) << ((blockReadData_[0] & 0xFF00) >> 8) << "\t" << (blockReadData_[0] & 0xFF) << std::endl;
		}
		else
		{
			ss << "        \t        " << std::endl;
		}
		if (blockReadData_.size() > 1)
		{
			ss << std::setw(8) << ((blockReadData_[1] & 0xFF00) >> 8) << "\t" << (blockReadData_[1] & 0xFF) << std::endl;
		}
		else
		{
			ss << "        \t        " << std::endl;
		}
		if (blockReadData_.size() > 2)
		{
			ss << std::setw(8) << ((blockReadData_[2] & 0xFF00) >> 8) << "\t" << (blockReadData_[2] & 0xFF) << std::endl;
		}
		else
		{
			ss << "        \t        " << std::endl;
		}
	}
	return ss.str();
}

DTCLib::DTC_DataPacket DTCLib::DTC_DCSReplyPacket::ConvertToDataPacket() const
{
	auto output = DTC_DMAPacket::ConvertToDataPacket();

	auto firstWord = (packetCount_ & 0x3FC) >> 2;
	auto secondWord = ((packetCount_ & 0x3) << 6) + (corruptFlag_ ? 0x20 : 0) + (dcsReceiveFIFOEmpty_ ? 0x10 : 0) +
		(requestAck_ ? 0x8 : 0) + (doubleOp_ ? 0x4 : 0) + static_cast<int>(type_);
	output.SetByte(4, static_cast<uint8_t>(secondWord));
	output.SetByte(5, static_cast<uint8_t>(firstWord));

	output.SetByte(6, static_cast<uint8_t>(address1_ & 0xFF));
	output.SetByte(7, static_cast<uint8_t>(((address1_ & 0xFF00) >> 8)));
	output.SetByte(8, static_cast<uint8_t>(data1_ & 0xFF));
	output.SetByte(9, static_cast<uint8_t>(((data1_ & 0xFF00) >> 8)));

	if (type_ != DTC_DCSOperationType_BlockRead)
	{
		output.SetByte(10, static_cast<uint8_t>(address2_ & 0xFF));
		output.SetByte(11, static_cast<uint8_t>(((address2_ & 0xFF00) >> 8)));
		output.SetByte(12, static_cast<uint8_t>(data2_ & 0xFF));
		output.SetByte(13, static_cast<uint8_t>(((data2_ & 0xFF00) >> 8)));
	}
	else
	{
		output.Resize((1 + packetCount_) * 16);
		size_t wordCounter = 10;
		for (auto& word : blockReadData_)
		{
			output.SetByte(wordCounter, word & 0xFF);
			output.SetByte(wordCounter + 1, (word & 0xFF00) >> 8);
			wordCounter += 2;
		}
	}
	return output;
}
