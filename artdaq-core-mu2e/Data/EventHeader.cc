#include "artdaq-core-mu2e/Data/EventHeader.hh"
#include <iomanip>
#include <ostream>
#include <cstdio>

namespace mu2e {

  bool EventHeader::isFlagBitSet( int bit) const{
    static constexpr std::array<uint8_t,8> mask = { 1, 2, 4, 8, 16, 32, 64, 128 };
    return ( (flags & mask.at(bit)) != 0 );
  }

  std::ostream& operator<<(std::ostream& os,
			   EventHeader const& eh ){

    // Represent flag bits in hex with prefixed 0x.
    // Printing it with <<, formats it as a char, which may be a non-printing character.
    //
    // Fixme: change to std::format when c++ 20 is available.
    // Also remove the temporaries then.
    char buf1[11];
    sprintf ( buf1, "0x%08X", eh.mode);
    char buf2[5];
    sprintf ( buf2, "0x%02X", int(eh.flags));

    os <<  "EWT: "           << std::setw(ewt_decimal_digits) << eh.ewt
       << " Event Mode: "    << buf1
       << " RF_Marker_TDC: " << int(eh.rfmTDC)
       << " Flags: "         << buf2;
    return os;
  }

}
