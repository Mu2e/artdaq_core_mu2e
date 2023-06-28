#include "artdaq-core-mu2e/Overlays/STMFragment.hh"


std::ostream& operator<<(std::ostream& os, const mu2e::STMFragment::STM_tHdr& tHdr)
{
	os << std::dec << "Data size is " << ((tHdr.sw_tHdr[2] << 16) | tHdr.sw_tHdr[3]) << "\n Test word is " << std::hex << tHdr.sw_tHdr[0] << 16 << tHdr.sw_tHdr[1] << std::endl;
	return os;
}
