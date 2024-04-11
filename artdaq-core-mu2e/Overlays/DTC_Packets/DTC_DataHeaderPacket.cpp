#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataHeaderPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Exceptions.h"

#include "TRACE/tracemf.h"

DTCLib::DTC_DataHeaderPacket::DTC_DataHeaderPacket(DTC_Link_ID link, uint16_t packetCount, uint8_t status,
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
		TLOG(TLVL_ERROR) << "Packet contents: " << in.toJSON();
		throw ex;
	}
	auto arr = in.GetData();
	packetCount_ = arr[4] + ((arr[5]&7) << 8);
	event_tag_ = DTC_EventWindowTag(arr, 6);
	status_ = arr[12];
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
	ss << headerJSON();
	ss << ",\n\t\"packetCount\": " << std::dec << static_cast<int>(packetCount_);
	ss << ",\n" << event_tag_.toJSON();
	ss << ",\n\t\"status\": " << std::dec << static_cast<int>(status_);
	ss << ",\n\t\"packetVersion\": " << std::hex << static_cast<int>(dataPacketVersion_);
	ss << ",\n\t\"DTC ID\": " << std::dec << static_cast<int>(dtcId_);
	ss << ",\n\t\"evbMode\": " << std::hex << "0x" << static_cast<int>(evbMode_);
	ss << "\n}";
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
	output.SetByte(4, static_cast<uint8_t>(packetCount_));
	output.SetByte(5, static_cast<uint8_t>((packetCount_ & 0x0700) >> 8));
	event_tag_.GetEventWindowTag(output.GetData(), 6);
	output.SetByte(12, static_cast<uint8_t>(status_));
	output.SetByte(13, static_cast<uint8_t>(dataPacketVersion_));
	output.SetByte(14, static_cast<uint8_t>(dtcId_));
	output.SetByte(15, evbMode_);
	return output;
}

bool DTCLib::DTC_DataHeaderPacket::Equals(const DTC_DataHeaderPacket& other) const
{
	return ConvertToDataPacket() == other.ConvertToDataPacket();
}
