#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_DMAPacket.h"

#include <iomanip>
#include <sstream>

CFOLib::CFO_DMAPacket::CFO_DMAPacket(CFO_PacketType type, 
	//DTC_Link_ID link, 
	uint16_t byteCount, bool valid
	//, uint8_t subsystemID, uint8_t hopCount
	)
	: byteCount_(byteCount), valid_(valid), 
	//subsystemID_(subsystemID), 
	//linkID_(link), 
	packetType_(type)
	//, hopCount_(hopCount) 
	{}

CFOLib::CFO_DataPacket CFOLib::CFO_DMAPacket::ConvertToDataPacket() const
{
	CFO_DataPacket output;
	output.Resize(byteCount_);
	auto word0A = static_cast<uint8_t>(byteCount_);
	auto word0B = static_cast<uint8_t>(byteCount_ >> 8);
	output.SetByte(0, word0A);
	output.SetByte(1, word0B);
	auto word1A = 0; //static_cast<uint8_t>(hopCount_ & 0xF);
	word1A += static_cast<uint8_t>(packetType_) << 4;
	uint8_t word1B = //static_cast<uint8_t>(linkID_ & 0x7) + 
		(valid_ ? 0x80 : 0x0)
		// + ((subsystemID_ & 0x7) << 4);
		;
	output.SetByte(2, word1A);
	output.SetByte(3, word1B);
	for (uint16_t i = 4; i < byteCount_; ++i)
	{
		output.SetByte(i, 0);
	}

	//std::cout << "ConvertToDataPacket: \n"
	//		  << output.toPacketFormat() << std::endl;

	return output;
}

CFOLib::CFO_DMAPacket::CFO_DMAPacket(const CFO_DataPacket in)
{
	auto word2 = in.GetData()[2];
	// uint8_t hopCount = word2 & 0xF;
	uint8_t packetType = word2 >> 4;
	auto word3 = in.GetData()[3];
	//uint8_t linkID = word3 & 0xF;
	valid_ = (word3 & 0x80) == 0x80;
	// subsystemID_ = (word3 >> 4) & 0x7;

	byteCount_ = in.GetData()[0] + (in.GetData()[1] << 8);
	// hopCount_ = hopCount;
	// linkID_ = static_cast<DTC_Link_ID>(linkID);
	packetType_ = static_cast<CFO_PacketType>(packetType);

	// This TRACE can be time-consuming!
#ifndef __OPTIMIZE__
	TLOG(TLVL_TRACE + 10, "CFO_DMAPacket") << headerJSON();
#endif
}

std::string CFOLib::CFO_DMAPacket::headerJSON() const
{
	std::stringstream ss;
	ss << "\t\"byteCount\": " << std::hex << "0x" << static_cast<int>(byteCount_) << ",\n";
	ss << "\t\"isValid\": " << valid_ << ",\n";
	// ss << "\t\"subsystemID\": " << std::hex << "0x" << static_cast<int>(subsystemID_) << ",\n";
	// ss << "\t\"linkID\": " << std::dec << linkID_ << ",\n";
	ss << "\t\"packetType\": " << packetType_ << ",\n";
	// ss << "\t\"hopCount\": " << std::hex << "0x" << static_cast<int>(hopCount_);

	return ss.str();
}

std::string CFOLib::CFO_DMAPacket::headerPacketFormat() const
{
	std::stringstream ss;
	ss << std::setfill('0') << std::hex;
	ss << "0x" << std::setw(6) << ((byteCount_ & 0xFF00) >> 8) << "\t"
		<< "0x" << std::setw(6) << (byteCount_ & 0xFF) << std::endl;
	ss << std::setw(1) << static_cast<int>(valid_) << " "
		//<< std::setw(2) << std::dec << static_cast<int>(subsystemID_) << std::hex << " "
		//<< "0x" << std::setw(2) << linkID_ 
		<< "\t";
	ss << "0x" << std::setw(2) << packetType_ << "0x" << std::setw(2) << 0 << std::endl;
	return ss.str();
}

std::string CFOLib::CFO_DMAPacket::toJSON()
{
	std::stringstream ss;
	ss << "\"DMAPacket\": {";
	ss << headerJSON();
	ss << "}";
	return ss.str();
}

std::string CFOLib::CFO_DMAPacket::toPacketFormat() { return headerPacketFormat(); }
