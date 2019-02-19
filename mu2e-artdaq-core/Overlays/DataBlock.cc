#include "mu2e-artdaq-core/Overlays/DataBlock.hh"

mu2e::DataBlock::DataBlock()
	: header_(nullptr), data_(nullptr) {}

mu2e::DataBlock::DataBlock(mu2e::packet_t* ptr)
	: header_(ptr), data_(ptr + 1) {}

mu2e::packet_t* mu2e::DataBlock::dataPacket(uint16_t index)
{
	if (index < getPacketCount()) {
		return &(data_[index]);
	}

	return nullptr;
}

uint16_t mu2e::DataBlock::getPacketCount() { return (*header_)[4] + ((*header_)[5] << 8); }

uint8_t mu2e::DataBlock::getROCID() { return ((*header_)[0] & 0xF) + (((*header_)[1] & 0xF) << 4); }
