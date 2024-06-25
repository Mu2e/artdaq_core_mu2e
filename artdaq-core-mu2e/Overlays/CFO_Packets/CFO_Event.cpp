#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_Event.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/Utilities.h"

#include "TRACE/tracemf.h"

#include <cstring>

//Copied structure from DTC_SubEvent.cpp

CFOLib::CFO_Event::CFO_Event(const void* data)
	: record_()
{
	memcpy(&record_, data, sizeof(record_));
}

// NOTE: do not need constructor for allocBytes because record_ is always a copy and memory is owned by instance

// NOTE: do not need SetupEvent because the CFO record is always received atomically in one buffer, and never spans multiple buffers

DTCLib::DTC_EventWindowTag CFOLib::CFO_Event::GetEventWindowTag() const
{
	return DTCLib::DTC_EventWindowTag(record_.event_tag);
}

void CFOLib::CFO_Event::SetEventWindowTag(DTCLib::DTC_EventWindowTag const& tag)
{
	record_.event_tag = tag.GetEventWindowTag(true);
}

void CFOLib::CFO_Event::SetEventMode(DTCLib::DTC_EventMode const& mode)
{
	uint64_t mode_word = mode.mode0;
	mode_word += (static_cast<uint64_t>(mode.mode1) << 8);
	mode_word += (static_cast<uint64_t>(mode.mode2) << 16);
	mode_word += (static_cast<uint64_t>(mode.mode3) << 24);
	mode_word += (static_cast<uint64_t>(mode.mode4) << 32);

	record_.event_mode = mode_word;
}
