#include "CFO_Packets.h"

#include <cstring>
#include <iomanip>
#include <sstream>

CFOLib::CFO_DataPacket::CFO_DataPacket()
{
	memPacket_ = false;
	vals_ = std::vector<uint8_t>(16);
	dataPtr_ = &vals_[0];
	dataSize_ = 16;
}

CFOLib::CFO_DataPacket::CFO_DataPacket(const CFO_DataPacket& in)
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

CFOLib::CFO_DataPacket::~CFO_DataPacket()
{
	if (!memPacket_ && dataPtr_ != nullptr)
	{
		dataPtr_ = nullptr;
	}
}

void CFOLib::CFO_DataPacket::SetByte(uint16_t index, uint8_t data)
{
	if (!memPacket_ && index < dataSize_)
	{
		const_cast<uint8_t*>(dataPtr_)[index] = data;
	}
}

uint8_t CFOLib::CFO_DataPacket::GetByte(uint16_t index) const
{
	if (index < dataSize_) return dataPtr_[index];
	return 0;
}

bool CFOLib::CFO_DataPacket::Resize(const uint16_t dmaSize)
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

std::string CFOLib::CFO_DataPacket::toJSON() const
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
	ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[jj]) << "]";
	ss << "}";
	return ss.str();
}

std::string CFOLib::CFO_DataPacket::toPacketFormat() const
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

bool CFOLib::CFO_DataPacket::Equals(const CFO_DataPacket& other) const
{
	auto equal = true;
	for (uint16_t ii = 2; ii < 16; ++ii)
	{
		// TRACE(21, "CFO_DataPacket::Equals: Compalink %u to %u", GetByte(ii), other.GetByte(ii));
		if (other.GetByte(ii) != GetByte(ii))
		{
			equal = false;
			break;
		}
	}

	return equal;
}

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


CFOLib::CFO_Event::CFO_Event(const void* data)
	: header_(), //sub_events_(), 
	buffer_ptr_(data)
{
	//memcpy(&header_, data, sizeof(header_));
}

CFOLib::CFO_Event::CFO_Event(size_t data_size)
	: allocBytes(new std::vector<uint8_t>(data_size)), header_(), 
	//sub_events_(), 
	buffer_ptr_(allocBytes->data())
{
	TLOG(TLVL_TRACE) << "Empty CFO_Event created, copy in data and call SetupEvent to finalize";
}

void CFOLib::CFO_Event::SetupEvent()
{
	auto ptr = reinterpret_cast<const uint8_t*>(buffer_ptr_);

	memcpy(&header_, ptr, sizeof(header_));
	ptr += sizeof(header_);

	// size_t byte_count = sizeof(header_);
	// while (byte_count < header_.inclusive_event_byte_count)
	// {
	// 	TLOG(TLVL_TRACE + 5) << "Current byte_count is " << byte_count << " / " << header_.inclusive_event_byte_count << ", creating sub event";
	// 	try {
	// 		sub_events_.emplace_back(ptr);
	// 		byte_count += sub_events_.back().GetSubEventByteCount();
	// 	}
	// 	catch (DTC_WrongPacketTypeException const& ex) {
	// 		TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up the event at location 0x" << std::hex << byte_count;
	// 		TLOG(TLVL_ERROR) << "This event has been truncated.";
	// 		break;
	// 	}
	// 	catch (DTC_WrongPacketSizeException const& ex) {
	// 		TLOG(TLVL_ERROR) << "A DTC_WrongPacketSizeException occurred while setting up the event at location 0x" << std::hex << byte_count;
	// 		TLOG(TLVL_ERROR) << "This event has been truncated.";
	// 		break;
	// 	}
	// }
}

DTCLib::DTC_EventWindowTag CFOLib::CFO_Event::GetEventWindowTag() const
{
	return DTC_EventWindowTag(header_.event_tag_low, header_.event_tag_high);
}

void CFOLib::CFO_Event::SetEventWindowTag(DTC_EventWindowTag const& tag)
{
	uint64_t tag_word = tag.GetEventWindowTag(true);
	header_.event_tag_low = tag_word;
	header_.event_tag_high = tag_word >> 32;
}

void CFOLib::CFO_Event::SetEventMode(DTC_EventMode const& mode)
{
	uint64_t mode_word = mode.mode0;
	mode_word += (static_cast<uint64_t>(mode.mode1) << 8);
	mode_word += (static_cast<uint64_t>(mode.mode2) << 16);
	mode_word += (static_cast<uint64_t>(mode.mode3) << 24);
	mode_word += (static_cast<uint64_t>(mode.mode4) << 32);

	header_.event_mode = mode_word;
}

// size_t WriteDMABufferSizeWords(std::ostream& output, bool includeDMAWriteSize, 
// 	size_t data_size, std::streampos& pos, bool restore_pos)
// {
// 	auto pos_save = output.tellp();
// 	output.seekp(pos);
// 	size_t bytes_written = 0;
// 	if (includeDMAWriteSize)
// 	{
// 		uint64_t dmaWriteSize = data_size + sizeof(uint64_t) + sizeof(uint64_t);
// 		output.write(reinterpret_cast<const char*>(&dmaWriteSize), sizeof(uint64_t));
// 		bytes_written += sizeof(uint64_t);
// 	}

// 	uint64_t dmaSize = data_size + sizeof(uint64_t);
// 	output.write(reinterpret_cast<const char*>(&dmaSize), sizeof(uint64_t));
// 	bytes_written += sizeof(uint64_t);
// 	if (restore_pos) {
// 		output.seekp(pos_save);
// 	}
// 	return bytes_written;
// }

void CFOLib::CFO_Event::WriteEvent(std::ostream& o, bool includeDMAWriteSize)
{
	// UpdateHeader();

	// if (header_.inclusive_event_byte_count + sizeof(uint64_t) + (includeDMAWriteSize ? sizeof(uint64_t) : 0) < MAX_DMA_SIZE) {
		TLOG(TLVL_TRACE) << "Event fits into one buffer, writing";
		auto pos = o.tellp();
		WriteDMABufferSizeWords(o, includeDMAWriteSize, 
			16, //header_.inclusive_event_byte_count, 
			pos, false);

		// o.write(reinterpret_cast<const char*>(&header_), sizeof(CFO_EventHeader));

		// for (auto& subevt : sub_events_)
		// {
		// 	o.write(reinterpret_cast<const char*>(subevt.GetHeader()), sizeof(CFO_SubEventHeader));
		// 	for (auto& blk : subevt.GetDataBlocks())
		// 	{
		// 		o.write(static_cast<const char*>(blk.blockPointer), blk.byteSize);
		// 	}
		// }
	// }
	// else {
	// 	TLOG(TLVL_TRACE) << "Event spans multiple buffers, beginning write";
	// 	auto buffer_start = o.tellp();
	// 	size_t bytes_written = WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count, buffer_start, false);

	// 	o.write(reinterpret_cast<const char*>(&header_), sizeof(CFO_EventHeader));
	// 	size_t buffer_data_size = sizeof(CFO_EventHeader);
	// 	size_t total_data_size = 0;

	// 	for (auto& subevt : sub_events_)
	// 	{
	// 		if (bytes_written + buffer_data_size + sizeof(CFO_SubEventHeader) > MAX_DMA_SIZE) {
	// 			TLOG(TLVL_TRACE) << "Starting new buffer, writing size words " << buffer_data_size << " to old buffer";
	// 			WriteDMABufferSizeWords(o, includeDMAWriteSize, buffer_data_size, buffer_start, true);
	// 			buffer_start = o.tellp();
	// 			total_data_size += buffer_data_size;
	// 			TLOG(TLVL_TRACE) << "Writing anticipated data size " << header_.inclusive_event_byte_count - total_data_size << " to new buffer.";
	// 			bytes_written = WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count - total_data_size, buffer_start, false);
	// 			buffer_data_size = 0;
	// 		}
	// 		o.write(reinterpret_cast<const char*>(subevt.GetHeader()), sizeof(CFO_SubEventHeader));
	// 		buffer_data_size += sizeof(CFO_SubEventHeader);
	// 		for (auto& blk : subevt.GetDataBlocks())
	// 		{
	// 			if (bytes_written + buffer_data_size + blk.byteSize > MAX_DMA_SIZE) {
	// 				TLOG(TLVL_TRACE) << "Starting new buffer, writing size words " << buffer_data_size << " to old buffer";
	// 				WriteDMABufferSizeWords(o, includeDMAWriteSize, buffer_data_size, buffer_start, true);
	// 				buffer_start = o.tellp();
	// 				total_data_size += buffer_data_size;
	// 				TLOG(TLVL_TRACE) << "Writing anticipated data size " << header_.inclusive_event_byte_count - total_data_size << " to new buffer.";
	// 				bytes_written = WriteDMABufferSizeWords(o, includeDMAWriteSize, header_.inclusive_event_byte_count - total_data_size, buffer_start, false);
	// 				buffer_data_size = 0;
	// 			}
	// 			o.write(static_cast<const char*>(blk.blockPointer), blk.byteSize);
	// 			buffer_data_size += blk.byteSize;
	// 		}
	// 	}
	// }
}
