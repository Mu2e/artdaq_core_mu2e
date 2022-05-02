#include "mu2e-artdaq-core/Data/SubRunHeader.hh"
#include <iomanip>
#include <ostream>

namespace mu2e {

  std::ostream& operator<<(std::ostream& os,
			   SubRunHeader const& sh ){
    os << " start Time: " << sh.startTime
       << " first EWT: "  << std::setw(ewt_decimal_digits) << sh.ewtFirst;
    return os;
  }

}
