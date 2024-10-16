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

		auto ptr = reinterpret_cast<const uint8_t*>(buffer_ptr_);
		TLOG(TLVL_ERROR) << "Subevent header raw data:";
		for(size_t i = 0; i < sizeof(header_); i += 4) 
				TLOG(TLVL_ERROR) << std::dec << "#" << i << "/" << sizeof(header_) << ": 0x" << 
					std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << std::endl;

		TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up a DTC Subevent in the header format version 0x" <<
			 std::hex << header_.subevent_format_version << " != 0x" << static_cast<uint16_t>(REQUIRED_SUBEVENT_FORMAT_VERSION) << 
			 ". Check that your DTC FPGA version matches the software expecation.";
		throw DTC_WrongPacketTypeException(REQUIRED_SUBEVENT_FORMAT_VERSION,header_.subevent_format_version);
	}
}

DTCLib::DTC_SubEvent::DTC_SubEvent(size_t data_size)
	: allocBytes(new std::vector<uint8_t>(data_size)), header_(), data_blocks_(), buffer_ptr_(allocBytes->data())
{
	TLOG(TLVL_TRACE) << "Empty DTC_SubEvent created, copy in data and call SetupSubEvent to finalize, data_size = " << data_size;
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
	TLOG(TLVL_TRACE) << "Inclusive SubEvent Byte Count is now " << header_.inclusive_subevent_byte_count << " for subevent " << static_cast<int>(GetDTCID());
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
		TLOG(TLVL_DEBUG + 6) <<	testss.str();
		TLOG(TLVL_DEBUG + 6) << header_.toJson();
	}
	ptr += sizeof(header_); //moving ptr past subevent header


	TLOG(TLVL_DEBUG + 6) << "Found sub event inclusive byte count as: " <<
		header_.inclusive_subevent_byte_count << " 0x" << 
		std::hex << std::setw(4) << std::setfill('0') << header_.inclusive_subevent_byte_count << ". i.e., " << std::dec << std::setw(0) << 
				(header_.inclusive_subevent_byte_count - sizeof(header_))/16 << " subevent packets.";

	size_t byte_count = sizeof(header_);
	uint8_t roc_fragi = -1;
	while (byte_count < header_.inclusive_subevent_byte_count)
	{	
		++roc_fragi;
		TLOG(TLVL_DEBUG + 6) << "Current byte_count is " << byte_count << " / " << header_.inclusive_subevent_byte_count << ", creating block";
		try 
		{
			data_blocks_.emplace_back(static_cast<const void*>(ptr));
			auto data_block_byte_count = data_blocks_.back().byteSize;
			byte_count += data_block_byte_count;
			TLOG(TLVL_DEBUG + 6) << "Found ROC fragment #" << static_cast<int>(roc_fragi) << " block of byte_count " << data_block_byte_count << " 0x" << 
				std::hex << data_block_byte_count << " (i.e., " << std::dec << 
				data_block_byte_count/16 << " fragment packets).";

			//printout ROC fragment data block
			if (data_block_byte_count > 16*2) //more than 2 packets 			
				TLOG(TLVL_DEBUG + 6) << "Beginning "
									<< std::hex << std::setw(8) << std::setfill('0') 
									<< *((uint32_t *)(&(ptr[0*4]))) << ' ' << *((uint32_t *)(&(ptr[1*4]))) << ' '
									<< *((uint32_t *)(&(ptr[2*4]))) << ' ' << *((uint32_t *)(&(ptr[3*4]))) << ' '
									<< *((uint32_t *)(&(ptr[4*4]))) << ' ' << *((uint32_t *)(&(ptr[5*4]))) << ' '
									<< *((uint32_t *)(&(ptr[6*4]))) << ' ' << *((uint32_t *)(&(ptr[7*4])));
			else
				TLOG(TLVL_DEBUG + 6) << "Beginning "
									<< std::hex << std::setw(8) << std::setfill('0') 
									<< *((uint32_t *)(&(ptr[0*4]))) << ' ' << *((uint32_t *)(&(ptr[1*4]))) << ' '
									<< *((uint32_t *)(&(ptr[2*4]))) << ' ' << *((uint32_t *)(&(ptr[3*4])));


			if (data_block_byte_count > 8*4) 
			{
				// save to print entire packet (for debugging and verifying emulator ROC):
				// for(size_t i = 0; i < data_block_byte_count; i += 4) 
				// {
				// 	std::cout << std::dec << "#" << i << "/" << data_block_byte_count << 
				// 		"(" << i/16 << "/" << data_block_byte_count/16 << ")" << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << std::endl;
				// 	if( i/16 > 0 && ((*((uint32_t *)(&(ptr[i]))) >> 4) & 0x0FFF) != (data_block_byte_count/16 - 1 - i/16))
				// 	{
				// 		TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up a ROC payload at packet" <<
				// 			i/16;
				// 		throw DTC_WrongPacketTypeException((data_block_byte_count/16 - 1 - i/16),((*((uint32_t *)(&(ptr[i]))) >> 4) & 0x0FFF));
				// 	}
				// }

				size_t i = data_block_byte_count-8*4;
				TLOG(TLVL_DEBUG + 6) << "End (starting at data block word #" << i << ") "
									<< std::hex << std::setw(8) << std::setfill('0') 
									<< *((uint32_t *)(&(ptr[i+0*4]))) << ' ' << *((uint32_t *)(&(ptr[i+1*4]))) << ' '
									<< *((uint32_t *)(&(ptr[i+2*4]))) << ' ' << *((uint32_t *)(&(ptr[i+3*4]))) << ' '
									<< *((uint32_t *)(&(ptr[i+4*4]))) << ' ' << *((uint32_t *)(&(ptr[i+5*4]))) << ' '
									<< *((uint32_t *)(&(ptr[i+6*4]))) << ' ' << *((uint32_t *)(&(ptr[i+7*4])));
            }

			if(data_blocks_.back().GetHeader()->GetLinkID() != roc_fragi)
			{
				TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException, mismatch of ROC Index, occurred while setting up a ROC header packet. Expected " << static_cast<int>(roc_fragi) << ", but data stream contained " << static_cast<int>(data_blocks_.back().GetHeader()->GetLinkID());
				throw DTC_WrongPacketTypeException(roc_fragi, data_blocks_.back().GetHeader()->GetLinkID());		
			}
			if(data_blocks_.back().GetHeader()->GetEventWindowTag().GetEventWindowTag(true) != GetEventWindowTag().GetEventWindowTag(true))
			{
				TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException, mismatch of ROC Event Tag, occurred while setting up a ROC #" << static_cast<int>(roc_fragi) << " header packet. Expected " << GetEventWindowTag().GetEventWindowTag(true) << ", but data stream contained " << data_blocks_.back().GetHeader()->GetEventWindowTag().GetEventWindowTag(true);
				throw DTC_WrongPacketTypeException(GetEventWindowTag().GetEventWindowTag(true), data_blocks_.back().GetHeader()->GetEventWindowTag().GetEventWindowTag(true));		
			}

			ptr += data_block_byte_count; //moving ptr past the ROC fragment data block
		}
		catch (DTC_WrongPacketTypeException const& ex)
		{
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketTypeException occurred while setting up a ROC Fragment #" << static_cast<int>(roc_fragi) <<
				" in the subevent at location " << byte_count <<  " / " << header_.inclusive_subevent_byte_count <<
				" 0x" << std::hex << byte_count << " / 0x" << header_.inclusive_subevent_byte_count;
			//printout SubEvent header
			{
				std::stringstream testss;
				testss << "ROC Data Header (w/overrun) for tag=" << GetEventWindowTag().GetEventWindowTag(true) << " (0x" << std::hex << 
					GetEventWindowTag().GetEventWindowTag(true) << ") bytes=" << std::dec << sizeof(header_) << ": 0x ";
				for(size_t i = 0; i < sizeof(header_); i+=4)
					testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << ' ';
				TLOG(TLVL_ERROR) <<	testss.str();
				TLOG(TLVL_ERROR) << header_.toJson();
			}
			{
				uint32_t roci = 0;
				for(auto& data_block : data_blocks_)
				{
					std::stringstream testss;
					testss << "ROC header #" << roci++ << 
						" ROC byte count = " << data_block.GetHeader()->GetByteCount() << ": 0x ";
					ptr = reinterpret_cast<const uint8_t*>(data_block.GetRawBufferPointer());
					for(size_t i = 0; i < sizeof(header_); i+=4)
						testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << ' ';
					testss << "\n End: ";
					for(size_t i = 0; i < sizeof(header_)*3; i+=4)
						testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i + data_block.GetHeader()->GetByteCount() - 128]))) << ' ';

					// testss << "\n All: ";
					// for(size_t i = 0; i < data_block.GetHeader()->GetByteCount(); i+=4)
					// {
					// 	if(i%( 24*4) == 0) testss << "\n" << i << "\t";
					// 	testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << ' ';
					// }
					
					std::cout << testss.str() << "\n" << std::flush;
				}
			}
			throw;
		}
		catch (DTC_WrongPacketSizeException const& ex) 
		{
			TLOG(TLVL_ERROR) << "A DTC_WrongPacketSizeException occurred while setting up a ROC Fragment #" << static_cast<int>(roc_fragi) <<
				" in the sub event at location " << byte_count <<  " / " << header_.inclusive_subevent_byte_count <<
				" 0x" << std::hex << byte_count << " / 0x" << header_.inclusive_subevent_byte_count;			
			//printout SubEvent header
			{
				std::stringstream testss;
				testss << "subevent header Tag=" << GetEventWindowTag().GetEventWindowTag(true) << " (0x" << std::hex << 
					GetEventWindowTag().GetEventWindowTag(true) << ") bytes=" << std::dec << sizeof(header_) << ": 0x ";
				for(size_t i = 0; i < sizeof(header_); i+=4)
					testss << std::hex << std::setw(8) << std::setfill('0') << *((uint32_t *)(&(ptr[i]))) << ' ';
				TLOG(TLVL_ERROR) <<	testss.str();
				TLOG(TLVL_ERROR) << header_.toJson();
			}
			throw;
		}
	}
	
} //end SetupSubEvent()
