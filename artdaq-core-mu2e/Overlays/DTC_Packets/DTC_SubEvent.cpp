#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_SubEvent.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Exceptions.h"

#include "TRACE/tracemf.h"

const uint8_t DTCLib::DTC_SubEvent::REQUIRED_SUBEVENT_FORMAT_VERSION = 1;

DTCLib::DTC_SubEvent::DTC_SubEvent(const void* data)
	: header_(), data_blocks_(), buffer_ptr_(data)
{
	memcpy(&header_, data, sizeof(header_));
	TLOG(TLVL_TRACE) << "Header of DTC_SubEvent created, copy in data and call SetupSubEvent to finalize";
	// Moved remainder to SetupSubEvent() to allow for SubEvents to cross DMA transfers
	if(header_.subevent_format_version != REQUIRED_SUBEVENT_FORMAT_VERSION)
	{
		TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up a DTC Subevent in the header format version 0x" <<
			 std::hex << header_.subevent_format_version << ". Check that your DTC FPGA version matches the software expecation.";
		throw DTC_WrongPacketTypeException(REQUIRED_SUBEVENT_FORMAT_VERSION,header_.subevent_format_version);
	}
}

DTCLib::DTC_SubEvent::DTC_SubEvent(size_t data_size)
	: allocBytes(new std::vector<uint8_t>(data_size)), header_(), data_blocks_(), buffer_ptr_(allocBytes->data())
{
	TLOG(TLVL_TRACE) << "Empty DTC_SubEvent created, copy in data and call SetupSubEvent to finalize";
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

void DTCLib::DTC_SubEvent::SetupSubEvent()
{
	auto ptr = reinterpret_cast<const uint8_t*>(buffer_ptr_);

	memcpy(&header_, ptr, sizeof(header_));
	if(header_.subevent_format_version != REQUIRED_SUBEVENT_FORMAT_VERSION)
	{
		TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up a DTC Subevent in the header format version 0x" <<
			 std::hex << header_.subevent_format_version << ". Check that your DTC FPGA version matches the software expecation.";
		throw DTC_WrongPacketTypeException(REQUIRED_SUBEVENT_FORMAT_VERSION,header_.subevent_format_version);
	}

	//printout SubEvent header
	{
		std::stringstream testss;
		testss << "subevent header Tag=" << GetEventWindowTag().GetEventWindowTag(true) << " (0x" << std::hex << 
			GetEventWindowTag().GetEventWindowTag(true) << ") bytes=" << std::dec << sizeof(header_) << ": 0x ";
		for(size_t i = 0; i < sizeof(header_); i+=4)
			testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << ' ';
		TLOG(TLVL_TRACE + 5) <<	testss.str();
	}
	ptr += sizeof(header_); //moving ptr past subevent header


	TLOG(TLVL_TRACE + 5) << "Found sub event inclusive byte count as: " <<
		header_.inclusive_subevent_byte_count << " 0x" << 
		std::hex << std::setw(4) << std::setfill('0') << header_.inclusive_subevent_byte_count << ". i.e., " << std::dec << 
				(header_.inclusive_subevent_byte_count - sizeof(header_))/16 << " subevent packets.";

	size_t byte_count = sizeof(header_);
	while (byte_count < header_.inclusive_subevent_byte_count)
	{
		TLOG(TLVL_TRACE + 5) << "Current byte_count is " << byte_count << " / " << header_.inclusive_subevent_byte_count << ", creating block";
		try 
		{
			data_blocks_.emplace_back(static_cast<const void*>(ptr));
			auto data_block_byte_count = data_blocks_.back().byteSize;
			byte_count += data_block_byte_count;
			TLOG(TLVL_TRACE + 5) << "Found ROC fragment block of byte_count " << data_block_byte_count << " 0x" << 
				std::hex << data_block_byte_count << " (i.e., " << std::dec << 
				data_block_byte_count/16 << " fragment packets).";

			//printout ROC fragment data block
			{
				std::stringstream testss;
				testss << "ROC fragment bytes=" << data_block_byte_count << " 0x ";
				for(size_t i = 0; i < data_block_byte_count; i+=4)
					testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << ' ';
				TLOG(TLVL_TRACE + 5) <<	testss.str();
			}
			ptr += data_block_byte_count; //moving ptr past the ROC fragment data block
		}
		catch (DTC_WrongPacketTypeException const& ex)
		{
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up a ROC Fragment in the sub event at location 0x" << std::hex << byte_count;
			throw;
		}
		catch (DTC_WrongPacketSizeException const& ex) 
		{
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketSizeException occurred while setting up a ROC Fragment in the sub event at location 0x" << std::hex << byte_count;
			throw;
		}
	}
	
} //end SetupSubEvent()
