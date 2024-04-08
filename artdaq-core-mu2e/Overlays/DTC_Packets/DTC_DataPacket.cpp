#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"

#include <iomanip>
#include <sstream>

DTCLib::DTC_DataPacket::DTC_DataPacket()
{
	memPacket_ = false;
	vals_ = std::vector<uint8_t>(16);
	dataPtr_ = &vals_[0];
	dataSize_ = 16;
}

DTCLib::DTC_DataPacket::DTC_DataPacket(const DTC_DataPacket& in)
{
	dataSize_ = in.GetSize();
	memPacket_ = in.IsMemoryPacket();
	if (!memPacket_)
	{
		vals_ = std::vector<uint8_t>(dataSize_);
		dataPtr_ = &vals_[0];
		memcpy(const_cast<uint8_t*>(dataPtr_), in.GetData(), in.GetSize() * sizeof(uint8_t));
	}
	else
	{
		dataPtr_ = in.GetData();
	}
}

DTCLib::DTC_DataPacket::~DTC_DataPacket()
{
	if (!memPacket_ && dataPtr_ != nullptr)
	{
		dataPtr_ = nullptr;
	}
}

void DTCLib::DTC_DataPacket::SetByte(uint16_t index, uint8_t data)
{
	if (!memPacket_ && index < dataSize_)
	{
		const_cast<uint8_t*>(dataPtr_)[index] = data;
	}
}

uint8_t DTCLib::DTC_DataPacket::GetByte(uint16_t index) const
{
	if (index < dataSize_) return dataPtr_[index];
	return 0;
}

bool DTCLib::DTC_DataPacket::Resize(const uint16_t dmaSize)
{
	if (!memPacket_ && dmaSize > dataSize_)
	{
		vals_.resize(dmaSize);
		dataPtr_ = &vals_[0];
		dataSize_ = dmaSize;
		return true;
	}

	// We can only grow, and only non-memory-mapped packets
	return false;
}

std::string DTCLib::DTC_DataPacket::toJSON() const
{
	std::stringstream ss;
	ss << "\"DataPacket\": {";
	ss << "\"data\": [";
	ss << std::hex << std::setfill('0');
	uint16_t jj = 0;
	for (uint16_t ii = 0; ii < dataSize_ - 2; ii += 2)
	{
		ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[jj]) << ",";
		++jj;
	}
	ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[jj]) << "]";
	ss << "}";
	return ss.str();
}

std::string DTCLib::DTC_DataPacket::toPacketFormat() const
{
	std::stringstream ss;
	ss << "\"data\": [";
	ss << std::hex << std::setfill('0');
	uint16_t jj = 0;
	for (uint16_t ii = 0; ii < dataSize_ - 2; ii += 2)
	{
		ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[jj]) << ",";
		++jj;
	}
	ss << "0x" << std::setw(4) << static_cast<int>(reinterpret_cast<uint16_t const*>(dataPtr_)[jj]) << "]";
	return ss.str();
}

bool DTCLib::DTC_DataPacket::Equals(const DTC_DataPacket& other) const
{
	auto equal = true;
	for (uint16_t ii = 2; ii < 16; ++ii)
	{
		// TRACE(21, "DTC_DataPacket::Equals: Compalink %u to %u", GetByte(ii), other.GetByte(ii));
		if (other.GetByte(ii) != GetByte(ii))
		{
			equal = false;
			break;
		}
	}

	return equal;
}
