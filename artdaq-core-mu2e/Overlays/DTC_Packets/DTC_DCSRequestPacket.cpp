#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DCSRequestPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Exceptions.h"

#include <cassert>

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
	ss << "\"DCSRequestPacket\": {\n";
	ss << headerJSON();
	ss << ",\n\"Operation Type\":" << DTC_DCSOperationTypeConverter(type_);
	ss << ",\n\"Request Acknowledgement\":" << (requestAck_ ? "\"true\"" : "\"false\"");
	ss << ",\n\"Address1\": " << static_cast<int>(address1_);
	if (type_ != DTC_DCSOperationType_BlockWrite)
	{
		ss << ",\n\"Data1\": " << static_cast<int>(data1_);
		ss << ",\n\"Address2\": " << static_cast<int>(address2_);
		ss << ",\n\"Data2\": " << static_cast<int>(data2_);
	}
	else
	{
		auto counter = 0;
		ss << ",\n\"Block Word Count\": " << static_cast<int>(data1_);
		for (auto& word : blockWriteData_)
		{
			ss << ",\n\"Block Write word " << counter << "\":" << static_cast<int>(word);
			counter++;
		}
	}
	ss << "\n}";

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
	byteCount_ = (packetCount_ + 1) * 16;
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
	output.SetByte(4, static_cast<uint8_t>(secondWord));
	output.SetByte(5, static_cast<uint8_t>(firstWord));

	output.SetByte(6, static_cast<uint8_t>(address1_ & 0xFF));
	output.SetByte(7, static_cast<uint8_t>(((address1_ & 0xFF00) >> 8)));
	output.SetByte(8, static_cast<uint8_t>(data1_ & 0xFF));
	output.SetByte(9, static_cast<uint8_t>(((data1_ & 0xFF00) >> 8)));

	if (type != DTC_DCSOperationType_BlockWrite)
	{
		output.SetByte(10, static_cast<uint8_t>(address2_ & 0xFF));
		output.SetByte(11, static_cast<uint8_t>(((address2_ & 0xFF00) >> 8)));
		output.SetByte(12, static_cast<uint8_t>(data2_ & 0xFF));
		output.SetByte(13, static_cast<uint8_t>(((data2_ & 0xFF00) >> 8)));
		output.SetByte(14, 0);
		output.SetByte(15, 0);
	}
	else
	{
		output.Resize((1 + packetCount_) * 16);
		size_t byteCounter = 10;
		for (auto& word : blockWriteData_)
		{
			output.SetByte(byteCounter, word & 0xFF);
			output.SetByte(byteCounter + 1, (word & 0xFF00) >> 8);
			byteCounter += 2;
		}
		for (; byteCounter < static_cast<size_t>((1 + packetCount_) * 16); byteCounter++)
		{
			output.SetByte(byteCounter, 0);
		}
	}
	return output;
}
