#ifndef ARTDAQ_CORE_MU2E_DATA_DTCDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_DTCDATADECODER_HH

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataBlock.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_SubEvent.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_SubEventHeader.h"
#include "cetlib_except/exception.h"

#include <iostream>
#include <vector>

// Implementation of "DTCDataDecoder", an artdaq::Fragment overlay class
// May contain multiple DataBlocks from the same ROC

namespace mu2e
{
struct DTCDataDecoder;
using DTCDataDecoders = std::vector<DTCDataDecoder>;

// Let the "<<" operator dump the DTCDataDecoder's data to stdout
std::ostream& operator<<(std::ostream&, DTCDataDecoder const&);
}  // namespace mu2e

struct mu2e::DTCDataDecoder
{
	explicit DTCDataDecoder(DTCLib::DTC_SubEvent const& se)
	    : event_(se)
	{
	}

	// const getter functions for the data in the header
	size_t block_count() const
	{
		return event_.GetDataBlockCount();
	}

	// Return size of block at given DataBlock index
	size_t blockSizeBytes(size_t blockIndex) const
	{
		if(blockIndex > block_count())
		{
			return 0;
		}

		return event_.GetDataBlock(blockIndex)->byteSize;
	}

	// Return pointer to beginning of DataBlock at given DataBlock index
	DTCLib::DTC_DataBlock const* dataAtBlockIndex(size_t blockIndex) const
	{
		if(blockIndex > block_count())
			return nullptr;
		return event_.GetDataBlock(blockIndex);
	}

	void printPacketAtByte(size_t blockIndex, size_t byteIdx) const
	{
		auto dataPtr = reinterpret_cast<uint16_t const*>(reinterpret_cast<uint8_t const*>(dataAtBlockIndex(blockIndex)->GetData()) + byteIdx);
		std::cout << "\t\t"
		          << "Packet Bits (128): " << std::endl;
		for(int adcIdx = 0; adcIdx < 8; adcIdx++)
		{
			std::cout << "\t";
			for(int offset = 15; offset >= 0; offset--)
			{
				if(((*(dataPtr + adcIdx)) & (1 << offset)) != 0)
				{
					std::cout << "1";
				}
				else
				{
					std::cout << "0";
				}
				if(offset == 8)
				{
					std::cout << " ";
				}
				else if(offset == 0)
				{
					std::cout << std::endl;
				}
			}
		}
		std::cout << std::endl;
		return;
	}

	DTCLib::DTC_SubEvent const& event_;
};

#endif /* mu2e_artdaq_Overlays_DTCDataDecoder_hh */
