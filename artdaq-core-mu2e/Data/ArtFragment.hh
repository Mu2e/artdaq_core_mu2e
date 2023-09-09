#ifndef mu2e_artdaq_core_Overlays_ArtFragment_hh
#define mu2e_artdaq_core_Overlays_ArtFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "artdaq-core/Data/dictionarycontrol.hh"
#include "cetlib_except/exception.h"
// #if HIDE_FROM_ROOT
#include "dtcInterfaceLib/DTC_Packets.h"
// #endif

#include <iostream>

#include <ostream>
#include <vector>

// Implementation of "ArtFragment", an artdaq::Fragment overlay class
// May contain multiple DataBlocks from the same ROC

namespace mu2e {
struct ArtFragment;
using ArtFragments = std::vector<ArtFragment>;

// Let the "<<" operator dump the ArtFragment's data to stdout
std::ostream &operator<<(std::ostream &, ArtFragment const &);
}  // namespace mu2e

struct mu2e::ArtFragment
{
	ArtFragment() {}

	explicit ArtFragment(std::vector<uint8_t> const& data)
		: data_(data) {
	}

  // #if HIDE_FROM_ROOT  // Hide most things from ROOT
	explicit ArtFragment(DTCLib::DTC_SubEvent const &se)
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
  // #endif
	
	mutable bool setup_{false};
	std::vector<uint8_t> data_;

  // #if HIDE_FROM_ROOT  // Hide most things from ROOT
	mutable DTCLib::DTC_SubEvent event_;  //! presume transient
  // #endif
};

#endif /* mu2e_artdaq_Overlays_ArtFragment_hh */
