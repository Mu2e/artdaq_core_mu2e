#ifndef mu2e_artdaq_core_Overlays_DataBlock_hh
#define mu2e_artdaq_core_Overlays_DataBlock_hh

///////////////////////////////////////////////////////////////////////
// DataBlock
//
// Class representing a mu2e "DataBlock", or DataHeaderPacket and its
// associated DataPackets. Each Fragment is made up of all DataBlocks
// from the ROCs connected to one DTC for one timestamp.
//
///////////////////////////////////////////////////////////////////////

#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"

#include <cstddef>
#include <cstdint>

namespace mu2e {
class DataBlock
{
public:
	DataBlock();
	DataBlock(packet_t* ptr);
	virtual ~DataBlock() {}

	packet_t* headerPacket() { return header_; }
	packet_t* dataBegin() { return data_; }
	packet_t* dataEnd() { return data_ + getPacketCount(); }
	packet_t* dataPacket(uint16_t index);

	uint16_t getPacketCount();
	uint8_t getROCID();

private:
	packet_t* header_;
	packet_t* data_;

};  // class DataBlock

}  // namespace mu2e

#endif
