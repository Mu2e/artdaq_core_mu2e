#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_Event.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Utilities.h"

#include "TRACE/tracemf.h"

#include <cstring>

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
	// 	TLOG(TLVL_DEBUG + 6) << "Current byte_count is " << byte_count << " / " << header_.inclusive_event_byte_count << ", creating sub event";
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
	return DTCLib::DTC_EventWindowTag(header_.event_tag_low, header_.event_tag_high);
}

void CFOLib::CFO_Event::SetEventWindowTag(DTCLib::DTC_EventWindowTag const& tag)
{
	uint64_t tag_word = tag.GetEventWindowTag(true);
	header_.event_tag_low = tag_word;
	header_.event_tag_high = tag_word >> 32;
}

void CFOLib::CFO_Event::SetEventMode(DTCLib::DTC_EventMode const& mode)
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
		DTCLib::Utilities::WriteDMABufferSizeWords(o, includeDMAWriteSize, 
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
