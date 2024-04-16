#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_HeartbeatPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Exceptions.h"

#include "TRACE/tracemf.h"

#include <sstream>

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
	output.SetByte(static_cast<uint16_t>(15), deliveryRingTDC_);
	return output;
}
