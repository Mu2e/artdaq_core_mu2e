#ifndef ARTDAQ_CORE_MU2E_DATA_DTCDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_DTCDATADECODER_HH

#include "cetlib_except/exception.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets.h"

#include <iostream>
#include <vector>

// Implementation of "DTCDataDecoder", an artdaq::Fragment overlay class
// May contain multiple DataBlocks from the same ROC

namespace mu2e {
struct DTCDataDecoder;
using DTCDataDecoders = std::vector<DTCDataDecoder>;

// Let the "<<" operator dump the DTCDataDecoder's data to stdout
std::ostream &operator<<(std::ostream &, DTCDataDecoder const &);
}  // namespace mu2e

struct mu2e::DTCDataDecoder
{
	DTCDataDecoder() {}

	explicit DTCDataDecoder(std::vector<uint8_t> const& data)
		: data_(data) {
	}

	explicit DTCDataDecoder(DTCLib::DTC_SubEvent const &se)
	{
		data_ = std::vector<uint8_t>(se.GetSubEventByteCount());
		memcpy(&data_[0], se.GetHeader(), sizeof(DTCLib::DTC_SubEventHeader));
		size_t offset = sizeof(DTCLib::DTC_SubEventHeader);

		for(auto& bl : se.GetDataBlocks()) {
			memcpy(&data_[0] + offset, bl.blockPointer, bl.byteSize);
			offset += bl.byteSize;
		}
		
		auto ptr = data_.data();
		event_ = DTCLib::DTC_SubEvent(ptr);	
		setup_ = true;
	}

	void setup_event() const {
		auto ptr = data_.data();
		event_ = DTCLib::DTC_SubEvent(ptr);	
		setup_ = true;
		}

	// const getter functions for the data in the header
	size_t block_count() const {
	  if (!setup_) {setup_event();}
	  return event_.GetDataBlockCount(); }

	// Return size of block at given DataBlock index
	size_t blockSizeBytes(size_t blockIndex) const
	{
		if (!setup_) setup_event();
		if (blockIndex > block_count())
		{
			return 0;
		}

		return event_.GetDataBlock(blockIndex)->byteSize;
	}

	// Return pointer to beginning of DataBlock at given DataBlock index
	DTCLib::DTC_DataBlock const *dataAtBlockIndex(size_t blockIndex) const
	{
		if (!setup_) setup_event();
		if (blockIndex > block_count()) return nullptr;
		return event_.GetDataBlock(blockIndex);
	}

	void printPacketAtByte(size_t blockIndex, size_t byteIdx) const
	{
		if (!setup_) setup_event();
		auto dataPtr = reinterpret_cast<uint16_t const *>(reinterpret_cast<uint8_t const *>(dataAtBlockIndex(blockIndex)->GetData()) + byteIdx);
		std::cout << "\t\t"
				  << "Packet Bits (128): " << std::endl;
		for (int adcIdx = 0; adcIdx < 8; adcIdx++)
		{
			std::cout << "\t";
			for (int offset = 15; offset >= 0; offset--)
			{
				if (((*(dataPtr + adcIdx)) & (1 << offset)) != 0)
				{
					std::cout << "1";
				}
				else
				{
					std::cout << "0";
				}
				if (offset == 8)
				{
					std::cout << " ";
				}
				else if (offset == 0)
				{
					std::cout << std::endl;
				}
			}
		}
		std::cout << std::endl;
		return;
	}
	
	mutable bool setup_{false};
	std::vector<uint8_t> data_;

	mutable DTCLib::DTC_SubEvent event_;  //! presume transient
};

#endif /* mu2e_artdaq_Overlays_DTCDataDecoder_hh */
