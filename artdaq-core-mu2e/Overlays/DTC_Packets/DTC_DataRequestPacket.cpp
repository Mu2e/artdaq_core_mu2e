#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataRequestPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Exceptions.h"

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
	output.SetByte(12, (static_cast<uint8_t>(type_) << 4) + (debug_ ? 1 : 0));
	output.SetByte(14, debugPacketCount_ & 0xFF);
	output.SetByte(15, (debugPacketCount_ >> 8) & 0xFF);
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

