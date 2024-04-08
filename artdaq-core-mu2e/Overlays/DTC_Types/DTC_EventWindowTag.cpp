#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventWindowTag.h"

#include <iomanip>
#include <sstream>

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag()
	: event_tag_(0) {}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const uint64_t event_tag)
	: event_tag_(event_tag) {}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const uint32_t event_tag_low, const uint16_t event_tag_high)
{
	SetEventWindowTag(event_tag_low, event_tag_high);
}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const uint8_t* timeArr, int offset)
{
	auto arr = reinterpret_cast<const uint64_t*>(timeArr + offset);
	event_tag_ = *arr;
}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const std::bitset<48> event_tag)
	: event_tag_(event_tag.to_ullong()) {}

void DTCLib::DTC_EventWindowTag::SetEventWindowTag(const uint32_t event_tag_low, const uint16_t event_tag_high)
{
	uint64_t event_tag_temp = event_tag_high;
	event_tag_temp = event_tag_temp << 32;
	event_tag_ = event_tag_low + event_tag_temp;
}

void DTCLib::DTC_EventWindowTag::GetEventWindowTag(const uint8_t* timeArr, int offset) const
{
	for (auto i = 0; i < 6; i++)
	{
		const_cast<uint8_t*>(timeArr)[i + offset] = static_cast<uint8_t>(event_tag_ >> i * 8);
	}
}

std::string DTCLib::DTC_EventWindowTag::toJSON(bool arrayMode) const
{
	std::stringstream ss;
	if (arrayMode)
	{
		uint8_t ts[6];
		GetEventWindowTag(ts, 0);
		ss << "\t\"timestamp\": [\n" << static_cast<int>(ts[0]) << ",\n";
		ss << static_cast<int>(ts[1]) << ",\n";
		ss << static_cast<int>(ts[2]) << ",\n";
		ss << static_cast<int>(ts[3]) << ",\n";
		ss << static_cast<int>(ts[4]) << ",\n";
		ss << static_cast<int>(ts[5]) << "\n]";
	}
	else
	{
		ss << "\t\"timestamp\": " << event_tag_;
	}
	return ss.str();
}

std::string DTCLib::DTC_EventWindowTag::toPacketFormat() const
{
	uint8_t ts[6]{0,0,0,0,0,0};
	GetEventWindowTag(ts, 0);
	std::stringstream ss;
	ss << std::setfill('0') << std::hex;
	ss << "0x" << std::setw(6) << static_cast<int>(ts[1]) << "\t"
	   << "0x" << std::setw(6) << static_cast<int>(ts[0]) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(ts[3]) << "\t"
	   << "0x" << std::setw(6) << static_cast<int>(ts[2]) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(ts[5]) << "\t"
	   << "0x" << std::setw(6) << static_cast<int>(ts[4]) << "\n";
	return ss.str();
}
