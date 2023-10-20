#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"

#include <iomanip>

std::ostream& mu2e::operator<<(std::ostream& os, DTCDataDecoder const& f)
{
	os << "DTCDataDecoder " << std::dec << ", block count: " << f.block_count() << "\n";
	return os;
}
