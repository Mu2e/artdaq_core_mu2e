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
	packetCount_ = arr[4] + ((arr[5] & 7) << 8);
	event_tag_ = DTC_EventWindowTag(arr, 6);
	status_ = arr[12];
	dataPacketVersion_ = arr[13];
	dtcId_ = arr[14];
	evbMode_ = arr[15];

	if ((packetCount_ + 1) * 16 != byteCount_)
	{
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
	ss << ",\n"
	   << event_tag_.toJSON();
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

bool DTCLib::DTC_DataHeaderPacket::IsDataHeaderPacket(const uint8_t* ptr, DTC_EventWindowTag timestamp, uint8_t dtc, DTC_Link_ID roc, DTC_Subsystem subsystem)
{
	uint64_t mask1 = 0x0000000080F00000;
	uint64_t mask2 = 0x0000000000000000;

	uint64_t comp1 = 0x0000000080500000;
	uint64_t comp2 = 0x0000000000000000;

	auto ptr64 = reinterpret_cast<const uint64_t*>(ptr);

	if (timestamp != DTC_EventWindowTag(static_cast<uint64_t>(0)))
	{
		TLOG(TLVL_DEBUG + 21) << "Checking ptr dtc=" << std::hex << std::showbase << dtc << " with mask " << mask2 << " and comp " << comp2;
		mask1 += 0xFFFF000000000000;
		mask2 += 0x00000000FFFFFFFF;

		auto tag = timestamp.GetEventWindowTag(true);
		comp1 += (tag & 0xFFFF) << 48;
		comp2 += (tag & 0xFFFFFFFF0000) >> 16;
		TLOG(TLVL_DEBUG + 21) << "Checking ptr dtc=" << std::hex << std::showbase << dtc << " with mask " << mask2 << " and comp " << comp2;
	}
	if (roc != DTC_Link_Unused)
	{
		mask1 += 0x0000000007000000;
		comp1 += static_cast<uint64_t>(roc) << 24;
	}
	if (subsystem != DTC_Subsystem_Unused)
	{
		mask1 += 0x0000E00000000000;
		comp1 += static_cast<uint64_t>(subsystem) << 45;
	}
	if (dtc != 0xFF)  // As of 12-Jan-2025, turning off DTC ID check (it was working) exception, demoted to error
	{
		TLOG(TLVL_DEBUG + 21) << "Checking ptr dtc=" << std::hex << std::showbase << (int)dtc << " with mask " << mask2 << " and comp " << comp2;

		// mask2 += static_cast<uint64_t>(0x0FF) << 48;
		// comp2 += static_cast<uint64_t>(dtc) << 48;
		if ((((*(ptr64 + 1)) >> 48) & 0xFF) != dtc)
		{
			TLOG(TLVL_DEBUG + 22) << "DTC ID check failed for ptr " << std::hex << std::showbase << *ptr64 << " " << *(ptr64 + 1) << ". Expected DTC ID " << std::hex << std::showbase << dtc << " in upper byte of second quad word, but found " << std::hex << std::showbase << ((*(ptr64 + 1)) >> 48);
		}

		TLOG(TLVL_DEBUG + 21) << "Checking ptr dtc=" << std::hex << std::showbase << (int)dtc << " with mask " << mask2 << " and comp " << comp2;
	}

	auto check1 = *ptr64 & mask1;
	auto check2 = *(ptr64 + 1) & mask2;

	TLOG(TLVL_DEBUG + 20) << "Checking ptr " << std::hex << std::showbase << *ptr64 << " " << *(ptr64 + 1) << " with masks " << mask1 << " " << mask2 << ". check1 " << check1 << " =?= " << comp1 << " comp1, check2 " << check2 << " =?= " << comp2 << " comp2";

	if (!(check1 == comp1 && check2 == comp2))
	{
		TLOG(TLVL_DEBUG) << "Failed header check! Looking for roc=" << roc << " and received " << std::hex << std::showbase << check1 << " =? " << comp1 << "  " << check2 << " =? " << comp2;
	}

	return check1 == comp1 && check2 == comp2;
}
