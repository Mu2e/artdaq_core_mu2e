#ifndef mu2e_artdaq_core_Overlays_ArtFragment_hh
#define mu2e_artdaq_core_Overlays_ArtFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"
#include "dtcInterfaceLib/DTC_Packets.h"

#include <iostream>

#include <ostream>
#include <vector>

// Implementation of "ArtFragment", an artdaq::Fragment overlay class
// May contain multiple DataBlocks from the same ROC

namespace mu2e {
class ArtFragment;

// Let the "<<" operator dump the ArtFragment's data to stdout
std::ostream &operator<<(std::ostream &, ArtFragment const &);
}  // namespace mu2e

class mu2e::ArtFragment
{
public:
	explicit ArtFragment(artdaq::Fragment const &f)
		: artdaq_Fragment_(f)
	{
		// Initialize the index array
		block_count_ = 0;
		auto curPos = dataBegin();
		if (dataBegin() != dataEnd())
		{
			while (curPos < dataEnd())
			{
				block_count_++;
				index_.push_back(new DTCLib::DTC_DataBlock(curPos));
				curPos += index_.back()->byteSize;
			}
		}
	}

	// Start of the DTC packets, returned as a pointer to the packet type
	uint8_t const *dataBegin() const
	{
		return reinterpret_cast<uint8_t const *>(&*artdaq_Fragment_.dataBegin());
	}

	uint8_t const *dataEnd() const
	{
		return reinterpret_cast<uint8_t const *>(&*artdaq_Fragment_.dataEnd());
	}

	// const getter functions for the data in the header
	size_t block_count() const { return block_count_; }

	// Return size of block at given DataBlock index
	size_t blockSizeBytes(size_t blockIndex) const
	{
		if (blockIndex > block_count())
		{
			return 0;
		}

		return index_[blockIndex]->byteSize;
	}

	// Return pointer to beginning of DataBlock at given DataBlock index
	DTCLib::DTC_DataBlock const *dataAtBlockIndex(size_t blockIndex) const
	{
		if (blockIndex > block_count()) return nullptr;
		return index_.at(blockIndex);
	}

	void printPacketAtByte(size_t blockIndex, size_t byteIdx) const
	{
		auto dataPtr = reinterpret_cast<uint16_t const*>(reinterpret_cast<uint8_t const*>(dataAtBlockIndex(blockIndex)->GetData()) + byteIdx);
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

private:
	artdaq::Fragment const &artdaq_Fragment_;
	size_t block_count_;
	std::vector<DTCLib::DTC_DataBlock const *> index_;
};

#endif /* mu2e_artdaq_Overlays_ArtFragment_hh */
