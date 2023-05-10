#ifndef _mu2e_artdaq_core_Mu2eEventHeader_hh_
#define _mu2e_artdaq_core_Mu2eEventHeader_hh_

#include <cstdint>

namespace mu2e {
class Mu2eEventHeader
{
public:
	explicit Mu2eEventHeader() {}
	Mu2eEventHeader(uint64_t timestamp, uint8_t mode);

public:
	uint64_t EventWindowTag;
	uint8_t EventMode;
};

}  // namespace mu2e
#endif  // _mu2e_artdaq_core_Mu2eEventHeader_hh_