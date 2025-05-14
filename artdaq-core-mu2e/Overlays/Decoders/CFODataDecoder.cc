#include "artdaq-core-mu2e/Overlays/Decoders/CFODataDecoder.hh"

#include "TRACE/tracemf.h"

#include <iostream>
#include <vector>

// Implementation of "CFODataDecoder"

namespace mu2e {

CFODataDecoder::CFODataDecoder(CFOLib::CFO_Event const &se)
	: event_(se)
{
}

}  // namespace mu2e
