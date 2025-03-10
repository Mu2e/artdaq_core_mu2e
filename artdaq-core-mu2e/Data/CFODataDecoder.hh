#ifndef ARTDAQ_CORE_MU2E_DATA_CFODATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CFODATADECODER_HH

#include "cetlib_except/exception.h"
#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_DMAPacket.h"
#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_Event.h"
#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_DataPacket.h"

#include <iostream>
#include <vector>

// Implementation of "CFODataDecoder"

namespace mu2e {
  struct CFODataDecoder;
  using  CFODataDecoders = std::vector<CFODataDecoder>;

  // Let the "<<" operator dump the CFODataDecoder's data to stdout
  std::ostream &operator<<(std::ostream &, CFODataDecoder const &);
}  // namespace mu2e

struct mu2e::CFODataDecoder
{
  CFODataDecoder() {}

  explicit CFODataDecoder(std::vector<uint8_t> const &data)
    : data_(data)
  {
  }

  explicit CFODataDecoder(CFOLib::CFO_Event const &se);

  void setup_event() const;

  // // const getter functions for the data in the header
  // size_t block_count() const
  // {
  // 	if (!setup_) { setup_event(); }
  // 	return event_.GetDataBlockCount();
  // }

  // Return size of block at given DataBlock index
  // size_t blockSizeBytes(size_t blockIndex) const
  // {
  // 	if (!setup_) setup_event();
  // 	// if (blockIndex > block_count())
  // 	// {
  // 	// 	return 0;
  // 	// }

  // 	return event_.GetDataBlock(blockIndex)->byteSize;
  // }

  // // Return pointer to beginning of DataBlock at given DataBlock index
  // CFOLib::CFO_DataBlock const *dataAtBlockIndex(size_t blockIndex) const
  // {
  // 	if (!setup_) setup_event();
  // 	if (blockIndex > block_count()) return nullptr;
  // 	return event_.GetDataBlock(blockIndex);
  // }

  // void printPacketAtByte(size_t blockIndex, size_t byteIdx) const
  // {
  // 	if (!setup_) setup_event();
  // 	auto dataPtr = reinterpret_cast<uint16_t const *>(reinterpret_cast<uint8_t const *>(dataAtBlockIndex(blockIndex)->GetData()) + byteIdx);
  // 	std::cout << "\t\t"
  // 			  << "Packet Bits (128): " << std::endl;
  // 	for (int adcIdx = 0; adcIdx < 8; adcIdx++)
  // 	{
  // 		std::cout << "\t";
  // 		for (int offset = 15; offset >= 0; offset--)
  // 		{
  // 			if (((*(dataPtr + adcIdx)) & (1 << offset)) != 0)
  // 			{
  // 				std::cout << "1";
  // 			}
  // 			else
  // 			{
  // 				std::cout << "0";
  // 			}
  // 			if (offset == 8)
  // 			{
  // 				std::cout << " ";
  // 			}
  // 			else if (offset == 0)
  // 			{
  // 				std::cout << std::endl;
  // 			}
  // 		}
  // 	}
  // 	std::cout << std::endl;
  // 	return;
  // }

  mutable bool setup_{false};
  std::vector<uint8_t> data_;

  mutable CFOLib::CFO_Event event_;  //! presume transient
};

#endif /* mu2e_artdaq_Data_CFODataDecoder_hh */
