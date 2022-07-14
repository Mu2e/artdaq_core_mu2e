#include "artdaq-core-mu2e/Overlays/DTCFragment.hh"

#include <iomanip>

std::ostream& mu2e::operator<<(std::ostream& os, DTCFragment const& f)
{
	os << "DTCFragment " << std::hex << ", packet count: " << f.hdr_packet_count() << ", timestamp: " << f.hdr_timestamp()
	   << "\n";

	return os;
}
