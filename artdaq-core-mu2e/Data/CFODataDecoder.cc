#include "artdaq-core-mu2e/Data/CFODataDecoder.hh"

#include "TRACE/tracemf.h"

#include <iostream>
#include <vector>

// Implementation of "CFODataDecoder"

namespace mu2e {

CFODataDecoder::CFODataDecoder(CFOLib::CFO_Event const &se)
{
	data_ = std::vector<uint8_t>(se.GetEventByteCount());
	memcpy(&data_[0], se.GetRawBufferPointer(), sizeof(CFOLib::CFO_Event));

	auto ptr = data_.data();
	event_ = CFOLib::CFO_Event(ptr);
	// event_.SetupSubEvent();
	setup_ = true;
}

void CFODataDecoder::setup_event() const
{
	auto ptr = data_.data();
	event_ = CFOLib::CFO_Event(ptr);
	setup_ = true;
}

}  // namespace mu2e
