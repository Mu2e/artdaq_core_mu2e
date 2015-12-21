#include "mu2e-artdaq-core/Overlays/mu2eFragment.hh"

#include <iomanip>
#include "cetlib/exception.h"

std::ostream & mu2e::operator << (std::ostream & os, mu2eFragment const & f) {
  os << "mu2eFragment " << std::dec
     << ", fragment count: " << f.hdr_fragment_count()
	 << ", type: " << f.hdr_fragment_type()
     << "\n";

  return os;
}
