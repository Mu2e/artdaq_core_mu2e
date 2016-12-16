#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"

#include <iomanip>
#include "cetlib/exception.h"

std::ostream & mu2e::operator << (std::ostream & os, DTCFragment const & f) {
  os << "DTCFragment " << std::hex
     << ", packet count: " << f.hdr_packet_count()
     << ", timestamp: "    << f.hdr_timestamp()
     << "\n";

  return os;
}
