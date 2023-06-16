#include "artdaq-core-mu2e/Data/ArtFragment.hh"

#include <iomanip>

std::ostream& mu2e::operator<<(std::ostream& os, ArtFragment const& f)
{
	os << "ArtFragment " << std::dec << ", block count: " << f.block_count() << "\n";
	return os;
}
