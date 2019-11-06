#ifndef mu2e_artdaq_core_Overlays_ArtFragment_hh
#define mu2e_artdaq_core_Overlays_ArtFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>

#include <ostream>
#include <vector>

// Implementation of "ArtFragment", an artdaq::Fragment overlay class

// The "packing factor": How many DataBlocks are stored in each ArtFragment
#define DATA_BLOCKS_PER_MU2E_FRAGMENT 2500

namespace mu2e {
class ArtFragment;

// Let the "<<" operator dump the ArtFragment's data to stdout
std::ostream &operator<<(std::ostream &, ArtFragment const &);
}  // namespace mu2e

class mu2e::ArtFragment
{
public:
	typedef uint16_t adc_t;

	ArtFragment(artdaq::Fragment const &f)
		: artdaq_Fragment_(f)
	{
		// Initialize the index array
		block_count_ = 0;
		size_t curIndex = 0;
		const adc_t *curPos = dataBegin();
		if (dataBegin() != dataEnd()) {
			while (curPos < dataEnd()) {
				block_count_++;
				// The first position in the index array corresponds to the second DataBlock (the first always has index 0)
				index[block_count_ - 1] = curIndex + *(curPos);
				curIndex += *(curPos);
				curPos = curPos + *(curPos) / 2;  // Increment curPos by the number of 16 bit values in the current DataBlock
			}
		}
		//    assert(block_count_ < BLOCK_COUNT_MAXIMUM, "Block count exceeds maximum");
	}

	// Start of the DTC packets, returned as a pointer to the packet type
	adc_t const *dataBegin() const
	{
		return reinterpret_cast<mu2e::ArtFragment::adc_t const *>(&*artdaq_Fragment_.dataBegin());
	}

	adc_t const *dataEnd() const
	{
		return reinterpret_cast<mu2e::ArtFragment::adc_t const *>(&*artdaq_Fragment_.dataEnd());
	}

	// const getter functions for the data in the header
	size_t block_count() const { return block_count_; }

	size_t byte_count() const { return index[block_count_ - 1]; }

	// Convert index in number of DataBlocks to index in bytes
	size_t blockIndexBytes(size_t offset) const
	{
		if (offset >= block_count()) {
			return 0;
		}
		else if (offset == 0)
		{
			return 0;
		}
		return index[offset - 1];
	}

	size_t blockEndBytes(size_t offset) const
	{
		if (offset >= block_count()) {
			return byte_count();
		}
		return index[offset];
	}

	// Return size of block at given DataBlock index
	size_t blockSizeBytes(size_t offset) const
	{
		if (offset > block_count() - 1) {
			return 0;
		}
		else if (offset == 0)
		{
			return index[offset];
		}
		return index[offset] - index[offset - 1];
	}

	// Return pointer to beginning of DataBlock at given byte index
	adc_t const *dataAtBytes(size_t offset) const { return dataBegin() + (offset / 2); }

	// Return pointer to beginning of DataBlock at given DataBlock index
	adc_t const *dataAtBlockIndex(size_t offset) const { return dataAtBytes(blockIndexBytes(offset)); }

	void printPacketAtByte(size_t byteIdx) const
	{
		std::cout << "\t\t"
				  << "Packet Bits (128): " << std::endl;
		for (int adcIdx = 0; adcIdx < 8; adcIdx++) {
			std::cout << "\t";
			for (int offset = 15; offset >= 0; offset--) {
				if (((*((adc_t const *)(dataAtBytes(byteIdx) + adcIdx))) & (1 << offset)) != 0) {
					std::cout << "1";
				}
				else
				{
					std::cout << "0";
				}
				if (offset == 8) {
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
	size_t index[DATA_BLOCKS_PER_MU2E_FRAGMENT];
};

#endif /* mu2e_artdaq_Overlays_ArtFragment_hh */
