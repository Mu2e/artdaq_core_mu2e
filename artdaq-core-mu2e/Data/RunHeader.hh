#ifndef mu2e_artdaq_core_Data_RunHeader_hh
#define mu2e_artdaq_core_Data_RunHeader_hh
//
// Header describing a Mu2e art::Run.
//
// Notes:
//  1) See Mu2e-doc-4914 for the definition of the information in the Heartbeat packet.
//  2) The time stamp comes from a system clock and is the time, in UTC, in seconds since the Unix 
//     epoch.  The epoch rollover is on Feb 27, 2106.
//  3) The time stamp will suffer from drift of the system clock from the NTP reference and the
//     slewing of the system clock when synching to NTP.  Both should normally be sub-second
//     but could be longer in a pathlogical case such as when the NTP connection has been down
//     for a long time.
//

#include "artdaq-core-mu2e/Data/EWT.hh"
#include "artdaq-core-mu2e/Data/TimeStamp.hh"
#include <iosfwd>

namespace mu2e {

struct RunHeader {

  RunHeader(){}

  RunHeader( time_t startTime, EWT ewtFirst ):
    startTime(startTime), ewtFirst(ewtFirst){
  }

  TimeStamp startTime;      // Time that the run started; seconds from the unix epoch in UTC.
  EWT       ewtFirst   = 0; // Event Window Tag of the first event in the Run.

};

  std::ostream& operator<<(std::ostream& os,
			   RunHeader const& rh );

}
#endif /* mu2e_artdaq_core_Data_RunHeader_hh */
