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
using CFODataDecoders = std::vector<CFODataDecoder>;

// Let the "<<" operator dump the CFODataDecoder's data to stdout
std::ostream &operator<<(std::ostream &, CFODataDecoder const &);
}  // namespace mu2e

struct mu2e::CFODataDecoder
{
	explicit CFODataDecoder(CFOLib::CFO_Event const &se);

	void setup_event() const;

	CFOLib::CFO_Event const& event_;
};

#endif /* mu2e_artdaq_Data_CFODataDecoder_hh */
