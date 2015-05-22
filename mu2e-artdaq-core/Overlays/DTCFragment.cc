#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"

#include "cetlib/exception.h"

std::ostream & mu2e::operator << (std::ostream & os, DTCFragment const & f) {
  os << "DTCFragment Timestamp: "
     << f.hdr_timestamp()
     << ", packet count: "
     << f.hdr_packet_count()
     << ", run number: "
     << f.hdr_run_number()
     << "\n";

  return os;
}

