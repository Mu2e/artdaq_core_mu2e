#include "artdaq-core-mu2e/Data/RunHeader.hh"
#include <iomanip>
#include <ostream>

namespace mu2e {

  std::ostream& operator<<(std::ostream& os,
			   RunHeader const& rh ){
    os << " start Time: " << rh.startTime
       << " first EWT: "  << std::setw(ewt_decimal_digits) << rh.ewtFirst;
    return os;
  }

}
