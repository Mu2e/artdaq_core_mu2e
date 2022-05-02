#include "mu2e-artdaq-core/Data/SubRunHeader.hh"
#include <ostream>

namespace mu2e {

  std::ostream& operator<<(std::ostream& os,
			   SubRunHeader const& sh ){
    os << " start Time: " << sh.startTime
       << " first EWT: "  << sh.ewtFirst;
    return os;
  }

}
