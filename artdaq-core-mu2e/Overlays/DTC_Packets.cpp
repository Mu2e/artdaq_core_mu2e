#include "DTC_Packets.h"

#include <cstring>
#include <iomanip>
#include <sstream>

DTCLib::DTC_DataPacket::DTC_DataPacket()
{
	memPacket_ = false;
	vals_ = std::vector<uint8_t>(16);
	dataPtr_ = &vals_[0];
	dataSize_ = 16;
}

DTCLib::DTC_DataPacket::DTC_DataPacket(const DTC_DataPacket& in)
{
	dataSize_ = in.GetSize();
	memPacket_ = in.IsMemoryPacket();
	if (!memPacket_)
	{
		vals_ = std::vector<uint8_t>(dataSize_);
		dataPtr_ = &vals_[0];
		memcpy(const_cast<uint8_t*>(dataPtr_), in.GetData(), in.GetSize() * sizeof(uint8_t));
	}
	else
	{
		dataPtr_ = in.GetData();
	}
}

DTCLib::DTC_DataPacket::~DTC_DataPacket()
{
	if (!memPacket_ && dataPtr_ != nullptr)
	{
		dataPtr_ = nullptr;
	}
}

void DTCLib::DTC_DataPacket::SetWord(uint16_t index, uint8_t data)
{
	if (!memPacket_ && index < dataSize_)
	{
		const_cast<uint8_t*>(dataPtr_)[index] = data;
	}
}

uint8_t DTCLib::DTC_DataPacket::GetWord(uint16_t index) const
{
	if (index < dataSize_) return dataPtr_[index];
	return 0;
}

bool DTCLib::DTC_DataPacket::Resize(const uint16_t dmaSize)
{
	if (!memPacket_ && dmaSize > dataSize_)
	{
		vals_.resize(dmaSize);
		dataPtr_ = &vals_[0];
		dataSize_ = dmaSize;
		return true;
	}

	// We can only grow, and only non-memory-mapped packets
	return false;
}

std::string DTCLib::DTC_DataPacket::toJSON() const
{
	std::stringstream ss;
	ss << "\"DataPacket\": {";
	ss << "\"data\": [";
	ss << std::hex << std::setfill('0');
	uint16_t jj = 0;
	for (uint16_t ii = 0; ii < dataSize_ - 2; ii += 2)
	{
		ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[jj]) << ",";
		++jj;
	}
	ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[dataSize_ - 2]) << "]";
	ss << "}";
	return ss.str();
}

std::string DTCLib::DTC_DataPacket::toPacketFormat() const
{
	std::stringstream ss;
	ss << std::setfill('0') << std::hex;
	for (uint16_t ii = 0; ii < dataSize_ - 1; ii += 2)
	{
		ss << "0x" << std::setw(2) << static_cast<int>(dataPtr_[ii + 1]) << " ";
		ss << "" << std::setw(2) << static_cast<int>(dataPtr_[ii]) << "\n";
	}
	ss << std::dec;
	return ss.str();
}

bool DTCLib::DTC_DataPacket::Equals(const DTC_DataPacket& other) const
{
	auto equal = true;
	for (uint16_t ii = 2; ii < 16; ++ii)
	{
		// TRACE(21, "DTC_DataPacket::Equals: Compalink %u to %u", GetWord(ii), other.GetWord(ii));
		if (other.GetWord(ii) != GetWord(ii))
		{
			equal = false;
			break;
		}
	}

	return equal;
}

DTCLib::DTC_DMAPacket::DTC_DMAPacket(DTC_PacketType type, DTC_Link_ID link, uint16_t byteCount, bool valid, uint8_t subsystemID, uint8_t hopCount)
	: byteCount_(byteCount), valid_(valid), subsystemID_(subsystemID), linkID_(link), packetType_(type), hopCount_(hopCount) {}

DTCLib::DTC_DataPacket DTCLib::DTC_DMAPacket::ConvertToDataPacket() const
{
	DTC_DataPacket output;
	output.Resize(byteCount_);
	auto word0A = static_cast<uint8_t>(byteCount_);
	auto word0B = static_cast<uint8_t>(byteCount_ >> 8);
	output.SetWord(0, word0A);
	output.SetWord(1, word0B);
	auto word1A = static_cast<uint8_t>(hopCount_ & 0xF);
	word1A += static_cast<uint8_t>(packetType_) << 4;
	uint8_t word1B = static_cast<uint8_t>(linkID_ & 0x7) + (valid_ ? 0x80 : 0x0) + ((subsystemID_ & 0x7) << 4);
	output.SetWord(2, word1A);
	output.SetWord(3, word1B);
	for (uint16_t i = 4; i < byteCount_; ++i)
	{
		output.SetWord(i, 0);
	}

	//std::cout << "ConvertToDataPacket: \n"
	//		  << output.toPacketFormat() << std::endl;

	return output;
}

DTCLib::DTC_DMAPacket::DTC_DMAPacket(const DTC_DataPacket in)
{
	auto word2 = in.GetData()[2];
	uint8_t hopCount = word2 & 0xF;
	uint8_t packetType = word2 >> 4;
	auto word3 = in.GetData()[3];
	uint8_t linkID = word3 & 0xF;
	valid_ = (word3 & 0x80) == 0x80;
	subsystemID_ = (word3 >> 4) & 0x7;

	byteCount_ = in.GetData()[0] + (in.GetData()[1] << 8);
	hopCount_ = hopCount;
	linkID_ = static_cast<DTC_Link_ID>(linkID);
	packetType_ = static_cast<DTC_PacketType>(packetType);

	// This TRACE can be time-consuming!
#ifndef __OPTIMIZE__
	TLOG(TLVL_TRACE + 10, "DTC_DMAPacket") << headerJSON();
#endif
}

std::string DTCLib::DTC_DMAPacket::headerJSON() const
{
	std::stringstream ss;
	ss << "\t\"byteCount\": " << std::hex << "0x" << static_cast<int>(byteCount_) << ",\n";
	ss << "\t\"isValid\": " << valid_ << ",\n";
	ss << "\t\"subsystemID\": " << std::hex << "0x" << static_cast<int>(subsystemID_) << ",\n";
	ss << "\t\"linkID\": " << std::dec << linkID_ << ",\n";
	ss << "\t\"packetType\": " << packetType_ << ",\n";
	ss << "\t\"hopCount\": " << std::hex << "0x" << static_cast<int>(hopCount_);

	return ss.str();
}

std::string DTCLib::DTC_DMAPacket::headerPacketFormat() const
{
	std::stringstream ss;
	ss << std::setfill('0') << std::hex;
	ss << "0x" << std::setw(6) << ((byteCount_ & 0xFF00) >> 8) << "\t"
		<< "0x" << std::setw(6) << (byteCount_ & 0xFF) << std::endl;
	ss << std::setw(1) << static_cast<int>(valid_) << " "
		<< std::setw(2) << std::dec << static_cast<int>(subsystemID_) << std::hex << " "
		<< "0x" << std::setw(2) << linkID_ << "\t";
	ss << "0x" << std::setw(2) << packetType_ << "0x" << std::setw(2) << 0 << std::endl;
	return ss.str();
}

std::string DTCLib::DTC_DMAPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"DMAPacket\": {";
	ss << headerJSON();
	ss << "}";
	return ss.str();
}

std::string DTCLib::DTC_DMAPacket::toPacketFormat() { return headerPacketFormat(); }

DTCLib::DTC_DCSRequestPacket::DTC_DCSRequestPacket()
	: DTC_DMAPacket(DTC_PacketType_DCSRequest, DTC_Link_Unused), type_(DTC_DCSOperationType_Unknown), packetCount_(0), address1_(0), data1_(0), address2_(0), data2_(0) {}

DTCLib::DTC_DCSRequestPacket::DTC_DCSRequestPacket(DTC_Link_ID link)
	: DTC_DMAPacket(DTC_PacketType_DCSRequest, link), type_(DTC_DCSOperationType_Unknown), packetCount_(0), address1_(0), data1_(0), address2_(0), data2_(0) {}

DTCLib::DTC_DCSRequestPacket::DTC_DCSRequestPacket(DTC_Link_ID link, DTC_DCSOperationType type, bool requestAck, bool incrementAddress,
	uint16_t address, uint16_t data, uint16_t address2, uint16_t data2)
	: DTC_DMAPacket(DTC_PacketType_DCSRequest, link), type_(type), requestAck_(requestAck), incrementAddress_(incrementAddress), packetCount_(0), address1_(address), data1_(data), address2_(address2), data2_(data2) {
	UpdatePacketAndWordCounts();
}

DTCLib::DTC_DCSRequestPacket::DTC_DCSRequestPacket(DTC_DataPacket in)
	: DTC_DMAPacket(in)
{
	if (packetType_ != DTC_PacketType_DCSRequest)
	{
		auto ex = DTC_WrongPacketTypeException(DTC_PacketType_DCSRequest, packetType_);
		TLOG(TLVL_ERROR) << ex.what();
		throw ex;
	}
	type_ = static_cast<DTC_DCSOperationType>(in.GetData()[4] & 0x7);
	requestAck_ = (in.GetData()[4] & 0x8) == 0x8;
	incrementAddress_ = (in.GetData()[4] & 0x10) == 0x10;

	packetCount_ = (in.GetData()[4] >> 6) + (in.GetData()[5] << 2);
	address1_ = in.GetData()[6] + (in.GetData()[7] << 8);
	data1_ = in.GetData()[8] + (in.GetData()[9] << 8);

	if (type_ == DTC_DCSOperationType_BlockWrite)
	{
		address2_ = 0;
		data2_ = 0;
		blockWriteData_.push_back(in.GetData()[10] + (in.GetData()[11] << 8));
		blockWriteData_.push_back(in.GetData()[12] + (in.GetData()[13] << 8));
		blockWriteData_.push_back(in.GetData()[14] + (in.GetData()[15] << 8));
	}
	else
	{
		address2_ = in.GetData()[10] + (in.GetData()[11] << 8);
		data2_ = in.GetData()[12] + (in.GetData()[13] << 8);
	}

	std::cout << "Constructor copy: " << toJSON() << std::endl;
}

std::string DTCLib::DTC_DCSRequestPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"DCSRequestPacket\": {";
	ss << headerJSON() << ", ";
	ss << "\"Operation Type\":" << DTC_DCSOperationTypeConverter(type_) << ", ";
	ss << "\"Request Acknowledgement\":" << (requestAck_ ? "\"true\"" : "\"false\"") << ", ";
	ss << "\"Address1\": " << static_cast<int>(address1_) << ", ";
	if (type_ != DTC_DCSOperationType_BlockWrite)
	{
		ss << "\"Data1\": " << static_cast<int>(data1_) << ", ";
		ss << "\"Address2\": " << static_cast<int>(address2_) << ", ";
		ss << "\"Data2\": " << static_cast<int>(data2_);
	}
	else
	{
		auto counter = 0;
		ss << ", \"Block Word Count\": " << static_cast<int>(data1_);
		for (auto& word : blockWriteData_)
		{
			ss << ", "
				<< "\"Block Write word " << counter << "\":" << static_cast<int>(word);
			counter++;
		}
	}
	ss << "}";

	return ss.str();
}

std::string DTCLib::DTC_DCSRequestPacket::toPacketFormat()
{
	std::stringstream ss;
	ss << headerPacketFormat() << std::hex << std::setfill('0');

	auto firstWord = (packetCount_ & 0x3FC) >> 2;
	auto secondWord =
		((packetCount_ & 0x3) << 6) + (incrementAddress_ ? 0x10 : 0) + (requestAck_ ? 0x8 : 0) + (static_cast<int>(type_) & 0x7);
	ss << std::setw(8) << firstWord << "\t" << secondWord << std::endl;

	ss << std::setw(8) << ((address1_ & 0xFF00) >> 8) << "\t" << (address1_ & 0xFF) << std::endl;
	ss << std::setw(8) << ((data1_ & 0xFF00) >> 8) << "\t" << (data1_ & 0xFF) << std::endl;
	if (type_ != DTC_DCSOperationType_BlockWrite)
	{
		ss << std::setw(8) << ((address2_ & 0xFF00) >> 8) << "\t" << (address2_ & 0xFF) << std::endl;
		ss << std::setw(8) << ((data2_ & 0xFF00) >> 8) << "\t" << (data2_ & 0xFF) << std::endl;
		ss << "        \t        " << std::endl;
	}
	else
	{
		if (blockWriteData_.size() > 0)
		{
			ss << std::setw(8) << ((blockWriteData_[0] & 0xFF00) >> 8) << "\t" << (blockWriteData_[0] & 0xFF) << std::endl;
		}
		else
		{
			ss << "        \t        " << std::endl;
		}
		if (blockWriteData_.size() > 1)
		{
			ss << std::setw(8) << ((blockWriteData_[1] & 0xFF00) >> 8) << "\t" << (blockWriteData_[1] & 0xFF) << std::endl;
		}
		else
		{
			ss << "        \t        " << std::endl;
		}
		if (blockWriteData_.size() > 2)
		{
			ss << std::setw(8) << ((blockWriteData_[2] & 0xFF00) >> 8) << "\t" << (blockWriteData_[2] & 0xFF) << std::endl;
		}
		else
		{
			ss << "        \t        " << std::endl;
		}
	}
	return ss.str();
}

void DTCLib::DTC_DCSRequestPacket::AddRequest(uint16_t address, uint16_t data)
{
	if (IsDoubleOp())
	{
		auto ex = DTC_IOErrorException(255);
		TLOG(TLVL_ERROR) << "DCS Request already has two requests, cannot add another! " << ex.what();
		throw ex;
	}
	if (type_ == DTC_DCSOperationType_BlockRead || type_ == DTC_DCSOperationType_BlockWrite)
	{
		auto ex = DTC_IOErrorException(254);
		TLOG(TLVL_ERROR) << "Cannot add second request to Block Read or Block Write operations! " << ex.what();
		throw ex;
	}
	type_ = static_cast<DTC_DCSOperationType>(type_ | 0x4);
	address2_ = address;
	data2_ = data;
}

void DTCLib::DTC_DCSRequestPacket::UpdatePacketAndWordCounts()
{
	assert(blockWriteData_.size() < 0x10000);

	if (type_ == DTC_DCSOperationType_BlockWrite)
	{
		data1_ = blockWriteData_.size();
	}

	if (type_ == DTC_DCSOperationType_BlockWrite && data1_ > 3)
	{
		packetCount_ = (data1_ - 3) / 8 + ((data1_ - 3) % 8 ? 1 : 0);
	}
	else
	{
		packetCount_ = 0;
	}
}

DTCLib::DTC_DataPacket DTCLib::DTC_DCSRequestPacket::ConvertToDataPacket() const
{
	auto output = DTC_DMAPacket::ConvertToDataPacket();

	auto type = type_;
	if (address2_ == 0 && data2_ == 0 && (type_ == DTC_DCSOperationType_DoubleRead || type_ == DTC_DCSOperationType_DoubleWrite))
	{
		type = static_cast<DTC_DCSOperationType>(type_ & 0x1);
	}

	auto firstWord = (packetCount_ & 0x3FC) >> 2;
	auto secondWord =
		((packetCount_ & 0x3) << 6) + (incrementAddress_ ? 0x10 : 0) + (requestAck_ ? 0x8 : 0) + (static_cast<int>(type) & 0x7);
	output.SetWord(4, static_cast<uint8_t>(secondWord));
	output.SetWord(5, static_cast<uint8_t>(firstWord));

	output.SetWord(6, static_cast<uint8_t>(address1_ & 0xFF));
	output.SetWord(7, static_cast<uint8_t>(((address1_ & 0xFF00) >> 8)));
	output.SetWord(8, static_cast<uint8_t>(data1_ & 0xFF));
	output.SetWord(9, static_cast<uint8_t>(((data1_ & 0xFF00) >> 8)));

	if (type != DTC_DCSOperationType_BlockWrite)
	{
		output.SetWord(10, static_cast<uint8_t>(address2_ & 0xFF));
		output.SetWord(11, static_cast<uint8_t>(((address2_ & 0xFF00) >> 8)));
		output.SetWord(12, static_cast<uint8_t>(data2_ & 0xFF));
		output.SetWord(13, static_cast<uint8_t>(((data2_ & 0xFF00) >> 8)));
		output.SetWord(14, 0);
		output.SetWord(15, 0);
	}
	else
	{
		output.Resize((1 + packetCount_) * 16);
		size_t wordCounter = 10;
		for (auto& word : blockWriteData_)
		{
			output.SetWord(wordCounter, word & 0xFF);
			output.SetWord(wordCounter + 1, (word & 0xFF00) >> 8);
			wordCounter += 2;
		}
		for (; wordCounter < static_cast<size_t>((1 + packetCount_) * 16); wordCounter++)
		{
			output.SetWord(wordCounter, 0);
		}
	}
	return output;
}

DTCLib::DTC_HeartbeatPacket::DTC_HeartbeatPacket(DTC_Link_ID link)
	: DTC_DMAPacket(DTC_PacketType_Heartbeat, link), event_tag_(), eventMode_(), deliveryRingTDC_()
{
	eventMode_.mode0 = 0;
	eventMode_.mode1 = 0;
	eventMode_.mode2 = 0;
	eventMode_.mode3 = 0;
	eventMode_.mode4 = 0;
}

DTCLib::DTC_HeartbeatPacket::DTC_HeartbeatPacket(DTC_Link_ID link, DTC_EventWindowTag event_tag, DTC_EventMode eventMode, uint8_t deliveryRingTDC)
	: DTC_DMAPacket(DTC_PacketType_Heartbeat, link), event_tag_(event_tag), eventMode_(eventMode), deliveryRingTDC_(deliveryRingTDC)
{}

DTCLib::DTC_HeartbeatPacket::DTC_HeartbeatPacket(const DTC_DataPacket in)
	: DTC_DMAPacket(in)
{
	if (packetType_ != DTC_PacketType_Heartbeat)
	{
		auto ex = DTC_WrongPacketTypeException(DTC_PacketType_Heartbeat, packetType_);
		TLOG(TLVL_ERROR) << ex.what();
		throw ex;
	}
	auto arr = in.GetData();
	eventMode_.mode0 = arr[10];
	eventMode_.mode1 = arr[11];
	eventMode_.mode2 = arr[12];
	eventMode_.mode3 = arr[13];
	eventMode_.mode4 = arr[14];
	deliveryRingTDC_ = arr[15];
	event_tag_ = DTC_EventWindowTag(arr, 4);
}

std::string DTCLib::DTC_HeartbeatPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"ReadoutRequestPacket\": {";
	ss << headerJSON() << ",";
	ss << event_tag_.toJSON() << ",";
	ss << "\"request\": [" << std::hex << "0x" << static_cast<int>(eventMode_.mode0) << ",";
	ss << std::hex << "0x" << static_cast<int>(eventMode_.mode1) << ",";
	ss << std::hex << "0x" << static_cast<int>(eventMode_.mode2) << ",";
	ss << std::hex << "0x" << static_cast<int>(eventMode_.mode3) << ",";
	ss << std::hex << "0x" << static_cast<int>(eventMode_.mode4) << "],";
	ss << "\"deliveryRingTDC\": " << std::hex << " 0x" << static_cast<int>(deliveryRingTDC_) << "";
	ss << "}";
	return ss.str();
}

std::string DTCLib::DTC_HeartbeatPacket::toPacketFormat()
{
	std::stringstream ss;
	ss << headerPacketFormat() << std::setfill('0') << std::hex;
	ss << event_tag_.toPacketFormat();
	ss << "0x" << std::setw(6) << static_cast<int>(eventMode_.mode1) << "\t0x" << std::setw(6)
		<< static_cast<int>(eventMode_.mode0) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(eventMode_.mode3) << "\t0x" << std::setw(6)
		<< static_cast<int>(eventMode_.mode2) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(deliveryRingTDC_) << "\t0x" << std::setw(6)
		<< static_cast<int>(eventMode_.mode4) << "\n";
	return ss.str();
}

DTCLib::DTC_DataPacket DTCLib::DTC_HeartbeatPacket::ConvertToDataPacket() const
{
	auto output = DTC_DMAPacket::ConvertToDataPacket();
	event_tag_.GetEventWindowTag(output.GetData(), 4);
	eventMode_.GetEventMode(output.GetData(), 10);
	output.SetWord(static_cast<uint16_t>(15), deliveryRingTDC_);
	return output;
}

DTCLib::DTC_DataRequestPacket::DTC_DataRequestPacket(DTC_Link_ID link, bool debug, uint16_t debugPacketCount,
	DTC_DebugType type)
	: DTC_DMAPacket(DTC_PacketType_DataRequest, link), event_tag_(), debug_(debug), debugPacketCount_(debugPacketCount), type_(type) {}

DTCLib::DTC_DataRequestPacket::DTC_DataRequestPacket(DTC_Link_ID link, DTC_EventWindowTag event_tag, bool debug,
	uint16_t debugPacketCount, DTC_DebugType type)
	: DTC_DMAPacket(DTC_PacketType_DataRequest, link), event_tag_(event_tag), debug_(debug), debugPacketCount_(debugPacketCount), type_(type) {}

DTCLib::DTC_DataRequestPacket::DTC_DataRequestPacket(DTC_DataPacket in)
	: DTC_DMAPacket(in)
{
	if (packetType_ != DTC_PacketType_DataRequest)
	{
		auto ex = DTC_WrongPacketTypeException(DTC_PacketType_DataRequest, packetType_);
		TLOG(TLVL_ERROR) << ex.what();
		throw ex;
	}
	event_tag_ = DTC_EventWindowTag(in.GetData(), 4);
	debug_ = (in.GetData()[12] & 0x1) == 1;
	type_ = DTC_DebugType((in.GetData()[12] & 0xF0) >> 4);
	debugPacketCount_ = in.GetData()[14] + (in.GetData()[15] << 8);
}

std::string DTCLib::DTC_DataRequestPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"DataRequestPacket\": {";
	ss << headerJSON() << ",";
	ss << event_tag_.toJSON() << ",";
	ss << "\"debug\":" << (debug_ ? "true" : "false") << ",";
	ss << "\"debugPacketCount\": " << std::dec << static_cast<int>(debugPacketCount_) << ",";
	ss << DTC_DebugTypeConverter(type_);
	ss << "}";
	return ss.str();
}

std::string DTCLib::DTC_DataRequestPacket::toPacketFormat()
{
	std::stringstream ss;
	ss << headerPacketFormat() << std::setfill('0') << std::hex;
	ss << event_tag_.toPacketFormat();
	ss << "        \t        \n";
	ss << "        \t0x" << std::setw(2) << static_cast<int>(type_) << "   " << std::setw(1) << static_cast<int>(debug_)
		<< "\n";
	ss << "0x" << std::setw(6) << ((debugPacketCount_ & 0xFF00) >> 8) << "\t"
		<< "0x" << std::setw(6) << (debugPacketCount_ & 0xFF) << "\n";
	return ss.str();
}

DTCLib::DTC_DataPacket DTCLib::DTC_DataRequestPacket::ConvertToDataPacket() const
{
	auto output = DTC_DMAPacket::ConvertToDataPacket();
	event_tag_.GetEventWindowTag(output.GetData(), 4);
	output.SetWord(12, (static_cast<uint8_t>(type_) << 4) + (debug_ ? 1 : 0));
	output.SetWord(14, debugPacketCount_ & 0xFF);
	output.SetWord(15, (debugPacketCount_ >> 8) & 0xFF);
	return output;
}

void DTCLib::DTC_DataRequestPacket::SetDebugPacketCount(uint16_t count)
{
	if (count > 0)
	{
		debug_ = true;
	}
	else
	{
		debug_ = false;
	}
	debugPacketCount_ = count;
}


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
// [31:16] Valid [15] DTC Errors [14:11] ROC Link ID [10:8] Packet Type (0x0) Hop Count [3:0]   |||| Common for all packets!
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
	DTCErrorBits_ = (in.GetData()[1] >> 3)  & 0xF;

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
	ss << "\"DCSReplyPacket\": {";
	ss << headerJSON() << ", ";
	ss << "\"Operation Type\":" << DTC_DCSOperationTypeConverter(type_) << ", ";
	ss << "\"Double Operation\":" << (doubleOp_ ? "\"true\"" : "\"false\"") << ", ";
	ss << "\"Request Acknowledgement\":" << (requestAck_ ? "\"true\"" : "\"false\"") << ", ";
	ss << "\"DCS Request FIFO Empty\": " << (dcsReceiveFIFOEmpty_ ? "\"true\"" : "\"false\"") << ", ";
	ss << "\"Corrupt Flag\": " << (corruptFlag_ ? "\"true\"" : "\"false\"") << ", ";
	ss << "\"Address1\": " << static_cast<int>(address1_) << ", ";
	if (type_ != DTC_DCSOperationType_BlockRead)
	{
		ss << "\"Data1\": " << static_cast<int>(data1_) << ", ";
		ss << "\"Address2\": " << static_cast<int>(address2_) << ", ";
		ss << "\"Data2\": " << static_cast<int>(data2_);
	}
	else
	{
		ss << "\"Block Word Count\": " << static_cast<int>(data1_);
		auto counter = 0;
		for (auto& word : blockReadData_)
		{
			ss << ", "
				<< "\"Block Read word " << counter << "\":" << static_cast<int>(word);
			counter++;
		}
	}
	ss << "}";
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
	output.SetWord(4, static_cast<uint8_t>(secondWord));
	output.SetWord(5, static_cast<uint8_t>(firstWord));

	output.SetWord(6, static_cast<uint8_t>(address1_ & 0xFF));
	output.SetWord(7, static_cast<uint8_t>(((address1_ & 0xFF00) >> 8)));
	output.SetWord(8, static_cast<uint8_t>(data1_ & 0xFF));
	output.SetWord(9, static_cast<uint8_t>(((data1_ & 0xFF00) >> 8)));

	if (type_ != DTC_DCSOperationType_BlockRead)
	{
		output.SetWord(10, static_cast<uint8_t>(address2_ & 0xFF));
		output.SetWord(11, static_cast<uint8_t>(((address2_ & 0xFF00) >> 8)));
		output.SetWord(12, static_cast<uint8_t>(data2_ & 0xFF));
		output.SetWord(13, static_cast<uint8_t>(((data2_ & 0xFF00) >> 8)));
	}
	else
	{
		output.Resize((1 + packetCount_) * 16);
		size_t wordCounter = 10;
		for (auto& word : blockReadData_)
		{
			output.SetWord(wordCounter, word & 0xFF);
			output.SetWord(wordCounter + 1, (word & 0xFF00) >> 8);
			wordCounter += 2;
		}
	}
	return output;
}

DTCLib::DTC_DataHeaderPacket::DTC_DataHeaderPacket(DTC_Link_ID link, uint16_t packetCount, DTC_DataStatus status,
	uint8_t dtcid, DTC_Subsystem subsystemid, uint8_t packetVersion, DTC_EventWindowTag event_tag,
	uint8_t evbMode)
	: DTC_DMAPacket(DTC_PacketType_DataHeader, link, (1 + packetCount) * 16, true, subsystemid), packetCount_(packetCount), event_tag_(event_tag), status_(status), dataPacketVersion_(packetVersion), dtcId_(dtcid), evbMode_(evbMode) {}

DTCLib::DTC_DataHeaderPacket::DTC_DataHeaderPacket(DTC_DataPacket in)
	: DTC_DMAPacket(in)
{
	if (packetType_ != DTC_PacketType_DataHeader)
	{
		auto ex = DTC_WrongPacketTypeException(DTC_PacketType_DataHeader, packetType_);
		TLOG(TLVL_ERROR) << "Unexpected packet type encountered: " + std::to_string(packetType_) + " != " + std::to_string(DTC_PacketType_DataHeader) +
			" (expected)";
		TLOG(TLVL_DEBUG) << "Packet contents: " << in.toJSON();
		throw ex;
	}
	auto arr = in.GetData();
	packetCount_ = arr[4] + (arr[5] << 8);
	event_tag_ = DTC_EventWindowTag(arr, 6);
	status_ = DTC_DataStatus(arr[12]);
	dataPacketVersion_ = arr[13];
	dtcId_ = arr[14];
	evbMode_ = arr[15];

	if ((packetCount_ + 1) * 16 != byteCount_) {
		auto ex = DTC_WrongPacketSizeException((packetCount_ + 1) * 16, byteCount_);
		TLOG(TLVL_ERROR) << "Unexpected packet size encountered: " + std::to_string((packetCount_ + 1) * 16) + " != " + std::to_string(byteCount_) +
			" (expected)";
		TLOG(TLVL_DEBUG) << "Packet contents: " << in.toJSON();
		throw ex;
	}
}

std::string DTCLib::DTC_DataHeaderPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"DataHeaderPacket\": {\n";
	ss << headerJSON() << ",\n";
	ss << "\t\"packetCount\": " << std::dec << static_cast<int>(packetCount_) << ",\n";
	ss << event_tag_.toJSON() << ",\n";
	ss << "\t\"status\": " << std::dec << static_cast<int>(status_) << ",\n";
	ss << "\t\"packetVersion\": " << std::hex << static_cast<int>(dataPacketVersion_) << ",\n";
	ss << "\t\"DTC ID\": " << std::dec << static_cast<int>(dtcId_) << ",\n";
	ss << "\t\"evbMode\": " << std::hex << "0x" << static_cast<int>(evbMode_) << "\n}";
	return ss.str();
}

std::string DTCLib::DTC_DataHeaderPacket::toPacketFormat()
{
	std::stringstream ss;
	ss << headerPacketFormat() << std::setfill('0') << std::hex;
	ss << "     0x" << std::setw(1) << ((packetCount_ & 0x0700) >> 8) << "\t"
		<< "0x" << std::setw(6) << (packetCount_ & 0xFF) << "\n";
	ss << event_tag_.toPacketFormat();
	ss << "0x" << std::setw(6) << static_cast<int>(dataPacketVersion_) << "\t"
		<< "0x" << std::setw(6) << static_cast<int>(status_) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(evbMode_) << "\t" << std::dec << std::setw(8) << static_cast<int>(dtcId_) << "\n";
	return ss.str();
}

DTCLib::DTC_DataPacket DTCLib::DTC_DataHeaderPacket::ConvertToDataPacket() const
{
	auto output = DTC_DMAPacket::ConvertToDataPacket();
	output.SetWord(4, static_cast<uint8_t>(packetCount_));
	output.SetWord(5, static_cast<uint8_t>((packetCount_ & 0x0700) >> 8));
	event_tag_.GetEventWindowTag(output.GetData(), 6);
	output.SetWord(12, static_cast<uint8_t>(status_));
	output.SetWord(13, static_cast<uint8_t>(dataPacketVersion_));
	output.SetWord(14, static_cast<uint8_t>(dtcId_));
	output.SetWord(15, evbMode_);
	return output;
}

bool DTCLib::DTC_DataHeaderPacket::Equals(const DTC_DataHeaderPacket& other) const
{
	return ConvertToDataPacket() == other.ConvertToDataPacket();
}

DTCLib::DTC_SubEvent::DTC_SubEvent(const uint8_t* ptr)
	: header_(), data_blocks_()
{
	memcpy(&header_, ptr, sizeof(header_));
	ptr += sizeof(header_);

	size_t byte_count = sizeof(header_);
	while (byte_count < header_.inclusive_subevent_byte_count)
	{
		TLOG(TLVL_TRACE + 5) << "Current byte_count is " << byte_count << " / " << header_.inclusive_subevent_byte_count << ", creating block";
		try {
			data_blocks_.emplace_back(static_cast<const void*>(ptr));
			auto data_block_byte_count = data_blocks_.back().byteSize;
			byte_count += data_block_byte_count;
			ptr += data_block_byte_count;
		}
		catch (DTC_WrongPacketTypeException const& ex) {
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up the sub event at location 0x" << std::hex << byte_count;
			throw;
		}
		catch (DTC_WrongPacketSizeException const& ex) {
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketSizeException occurred while setting up the sub event at location 0x" << std::hex << byte_count;
			throw;
		}
	}
}

DTCLib::DTC_EventWindowTag DTCLib::DTC_SubEvent::GetEventWindowTag() const
{
	return DTC_EventWindowTag(header_.event_tag_low, header_.event_tag_high);
}

void DTCLib::DTC_SubEvent::SetEventWindowTag(DTC_EventWindowTag const& tag)
{
	uint64_t tag_word = tag.GetEventWindowTag(true);
	header_.event_tag_low = tag_word;
	header_.event_tag_high = tag_word >> 32;
}

void DTCLib::DTC_SubEvent::SetEventMode(DTC_EventMode const& mode)
{
	uint64_t mode_word = mode.mode0;
	mode_word += (static_cast<uint64_t>(mode.mode1) << 8);
	mode_word += (static_cast<uint64_t>(mode.mode2) << 16);
	mode_word += (static_cast<uint64_t>(mode.mode3) << 24);
	mode_word += (static_cast<uint64_t>(mode.mode4) << 32);

	header_.event_mode = mode_word;
}

uint8_t DTCLib::DTC_SubEvent::GetDTCID() const
{
	return header_.source_dtc_id;
}

void DTCLib::DTC_SubEvent::UpdateHeader()
{
	header_.inclusive_subevent_byte_count = sizeof(DTC_SubEventHeader);

	for (auto& block : data_blocks_)
	{
		header_.inclusive_subevent_byte_count += block.byteSize;
	}
	TLOG(TLVL_TRACE) << "Inclusive SubEvent Byte Count is now " << header_.inclusive_subevent_byte_count;
}

DTCLib::DTC_Event::DTC_Event(const void* data)
	: header_(), sub_events_(), buffer_ptr_(data)
{
	memcpy(&header_, data, sizeof(header_));
}

DTCLib::DTC_Event::DTC_Event(size_t data_size)
	: allocBytes(new std::vector<uint8_t>(data_size)), header_(), sub_events_(), buffer_ptr_(allocBytes->data())
{
	TLOG(TLVL_TRACE) << "Empty DTC_Event created, copy in data and call SetupEvent to finalize";
}

void DTCLib::DTC_Event::SetupEvent()
{
	auto ptr = reinterpret_cast<const uint8_t*>(buffer_ptr_);

	memcpy(&header_, ptr, sizeof(header_));
	ptr += sizeof(header_);

	size_t byte_count = sizeof(header_);
	while (byte_count < header_.inclusive_event_byte_count)
	{
		TLOG(TLVL_TRACE + 5) << "Current byte_count is " << byte_count << " / " << header_.inclusive_event_byte_count << ", creating sub event";
		try {
			sub_events_.emplace_back(ptr);
			byte_count += sub_events_.back().GetSubEventByteCount();
		}
		catch (DTC_WrongPacketTypeException const& ex) {
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up the event at location 0x" << std::hex << byte_count;
			TLOG(TLVL_ERROR) << "This event has been truncated.";
			break;
		}
		catch (DTC_WrongPacketSizeException const& ex) {
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketSizeException occurred while setting up the event at location 0x" << std::hex << byte_count;
			TLOG(TLVL_ERROR) << "This event has been truncated.";
			break;
		}
	}
}

DTCLib::DTC_EventWindowTag DTCLib::DTC_Event::GetEventWindowTag() const
{
	return DTC_EventWindowTag(header_.event_tag_low, header_.event_tag_high);
}

void DTCLib::DTC_Event::SetEventWindowTag(DTC_EventWindowTag const& tag)
{
	uint64_t tag_word = tag.GetEventWindowTag(true);
	header_.event_tag_low = tag_word;
	header_.event_tag_high = tag_word >> 32;
}

void DTCLib::DTC_Event::SetEventMode(DTC_EventMode const& mode)
{
	uint64_t mode_word = mode.mode0;
	mode_word += (static_cast<uint64_t>(mode.mode1) << 8);
	mode_word += (static_cast<uint64_t>(mode.mode2) << 16);
	mode_word += (static_cast<uint64_t>(mode.mode3) << 24);
	mode_word += (static_cast<uint64_t>(mode.mode4) << 32);

	header_.event_mode = mode_word;
}

void DTCLib::DTC_Event::UpdateHeader()
{
	header_.inclusive_event_byte_count = sizeof(DTC_EventHeader);
	for (auto& sub_evt : sub_events_)
	{
		sub_evt.UpdateHeader();
		header_.inclusive_event_byte_count += sub_evt.GetSubEventByteCount();
	}
	TLOG(TLVL_TRACE) << "Inclusive Event Byte Count is now " << header_.inclusive_event_byte_count;
}

size_t WriteDMABufferSizeWords(std::ostream& output, bool includeDMAWriteSize, size_t data_size, std::streampos& pos, bool restore_pos)
{
	auto pos_save = output.tellp();
	output.seekp(pos);
	size_t bytes_written = 0;
	if (includeDMAWriteSize)
	{
		uint64_t dmaWriteSize = data_size + sizeof(uint64_t) + sizeof(uint64_t);
		output.write(reinterpret_cast<const char*>(&dmaWriteSize), sizeof(uint64_t));
		bytes_written += sizeof(uint64_t);
	}

	uint64_t dmaSize = data_size + sizeof(uint64_t);
	output.write(reinterpret_cast<const char*>(&dmaSize), sizeof(uint64_t));
	bytes_written += sizeof(uint64_t);
	if (restore_pos) {
		output.seekp(pos_save);
	}
	return bytes_written;
}

void DTCLib::DTC_Event::WriteEvent(std::ostream& o, bool includeDMAWriteSize)
{
	UpdateHeader();

	if (header_.inclusive_event_byte_count + sizeof(uint64_t) + (includeDMAWriteSize ? sizeof(uint64_t) : 0) < MAX_DMA_SIZE) {
		TLOG(TLVL_TRACE) << "Event fits into one buffer, writing";
		auto pos = o.tellp();
		WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count, pos, false);

		o.write(reinterpret_cast<const char*>(&header_), sizeof(DTC_EventHeader));

		for (auto& subevt : sub_events_)
		{
			o.write(reinterpret_cast<const char*>(subevt.GetHeader()), sizeof(DTC_SubEventHeader));
			for (auto& blk : subevt.GetDataBlocks())
			{
				o.write(static_cast<const char*>(blk.blockPointer), blk.byteSize);
			}
		}
	}
	else {
		TLOG(TLVL_TRACE) << "Event spans multiple buffers, beginning write";
		auto buffer_start = o.tellp();
		size_t bytes_written = WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count, buffer_start, false);

		o.write(reinterpret_cast<const char*>(&header_), sizeof(DTC_EventHeader));
		size_t buffer_data_size = sizeof(DTC_EventHeader);
		size_t total_data_size = 0;

		for (auto& subevt : sub_events_)
		{
			if (bytes_written + buffer_data_size + sizeof(DTC_SubEventHeader) > MAX_DMA_SIZE) {
				TLOG(TLVL_TRACE) << "Starting new buffer, writing size words " << buffer_data_size << " to old buffer";
				WriteDMABufferSizeWords(o, includeDMAWriteSize, buffer_data_size, buffer_start, true);
				buffer_start = o.tellp();
				total_data_size += buffer_data_size;
				TLOG(TLVL_TRACE) << "Writing anticipated data size " << header_.inclusive_event_byte_count - total_data_size << " to new buffer.";
				bytes_written = WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count - total_data_size, buffer_start, false);
				buffer_data_size = 0;
			}
			o.write(reinterpret_cast<const char*>(subevt.GetHeader()), sizeof(DTC_SubEventHeader));
			buffer_data_size += sizeof(DTC_SubEventHeader);
			for (auto& blk : subevt.GetDataBlocks())
			{
				if (bytes_written + buffer_data_size + blk.byteSize > MAX_DMA_SIZE) {
					TLOG(TLVL_TRACE) << "Starting new buffer, writing size words " << buffer_data_size << " to old buffer";
					WriteDMABufferSizeWords(o, includeDMAWriteSize, buffer_data_size, buffer_start, true);
					buffer_start = o.tellp();
					total_data_size += buffer_data_size;
					TLOG(TLVL_TRACE) << "Writing anticipated data size " << header_.inclusive_event_byte_count - total_data_size << " to new buffer.";
					bytes_written = WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count - total_data_size, buffer_start, false);
					buffer_data_size = 0;
				}
				o.write(static_cast<const char*>(blk.blockPointer), blk.byteSize);
				buffer_data_size += blk.byteSize;
			}
		}
	}
}

std::string DTCLib::DTC_SubEventHeader::toJson() const
{
	std::ostringstream oss;

	oss << "\"DTC_SubEventHeader\": {\n";
	oss << "\t\"inclusive_subevent_byte_count\": " << inclusive_subevent_byte_count << ",\n";
	oss << "\t\"event_tag_low\": " << event_tag_low << ",\n";
	oss << "\t\"event_tag_high\": " << event_tag_high << ",\n";
	oss << "\t\"num_rocs\": " << num_rocs << ",\n";
	oss << "\t\"event_mode\": 0x" << std::hex << event_mode << ",\n";
	oss << "\t\"dtc_mac\": " << dtc_mac << ",\n";
	oss << "\t\"partition_id\": " << partition_id << ",\n";
	oss << "\t\"evb_mode\": " << evb_mode << ",\n";
	oss << "\t\"source_dtc_id\": " << source_dtc_id << ",\n";
	oss << "\t\"link0_status\": " << link0_status << ",\n";
	oss << "\t\"link1_status\": " << link1_status << ",\n";
	oss << "\t\"link2_status\": " << link2_status << ",\n";
	oss << "\t\"link3_status\": " << link3_status << ",\n";
	oss << "\t\"link4_status\": " << link4_status << ",\n";
	oss << "\t\"link5_status\": " << link5_status << ",\n";
	oss << "\t\"emtdc\": " << emtdc << "\n}";

	return oss.str();
}

std::string DTCLib::DTC_EventHeader::toJson() const
{
	std::ostringstream oss;

	oss << "\"DTC_EventHeader\": {\n";
	oss << "\t\"inclusive_event_byte_count\": " << inclusive_event_byte_count << ",\n";
	oss << "\t\"event_tag_low\": " << event_tag_low << ",\n";
	oss << "\t\"event_tag_high\": " << event_tag_high << ",\n";
	oss << "\t\"num_dtcs\": " << num_dtcs << ",\n";
	oss << "\t\"event_mode\": 0x" << std::hex << event_mode << ",\n";
	oss << "\t\"dtc_mac\": " << dtc_mac << ",\n";
	oss << "\t\"partition_id\": " << partition_id << ",\n";
	oss << "\t\"evb_mode\": " << evb_mode << ",\n";
	oss << "\t\"evb_id\": " << evb_id << ",\n";
	oss << "\t\"evb_status\": " << evb_status << ",\n";
	oss << "\t\"emtdc\": " << emtdc << "\n}";

	return oss.str();
}
