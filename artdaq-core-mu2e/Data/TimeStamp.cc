#include "artdaq-core-mu2e/Data/TimeStamp.hh"
#include <iomanip>
#include <ostream>

namespace mu2e {

  // Print time, in UTC, as a formatted string, including the time zone specifier.
  std::ostream& operator<<(std::ostream& os,
			   TimeStamp const& ts ){
    auto t = ts.get();
    os << std::put_time(gmtime(&t),"%FT%T%z");
    return os;
  }

}
