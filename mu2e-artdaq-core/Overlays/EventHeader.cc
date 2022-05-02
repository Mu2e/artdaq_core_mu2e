#include "mu2e-artdaq-core/Overlays/EventHeader.hh"
#include <ostream>

namespace mu2e {

  std::ostream& operator<<(std::ostream& os,
			   EventHeader const& eh ){
    os <<  "EWT: "           << eh.ewt
       << " Mode: "          << eh.mode
       << " RF Marker TDC: " << eh.rfmTDC
       << " Flags: "         << eh.flags;
    return os;
  }

}
