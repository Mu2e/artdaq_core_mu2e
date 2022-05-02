#ifndef mu2e_artdaq_core_Data_EventHeader_hh
#define mu2e_artdaq_core_Data_EventHeader_hh
//
// Header describing a Mu2e art::Event
//
// Notes:
//  1) See Mu2e-doc-4914 for the definition of the informaton in the Heartbeat packet.
//  2) EventWindowTag is redundant with the art::EventId but having it will be useful for
//     consistency checking, particularly during commissioning and after major
//     upgrades to TDAQ.
//     Cost to keep this: about 200 GB in Raw data files summed over nominal Mu2e running.
//     Estimate O(10) times that summed over all derived data products.
//  3) Did not use std::bitset for flags since, in  g++ 9.3.0, it allocates space in chunks of 32 bits.
//  4) Fixme: add additional named bits in flags and EventMode as they are defined; maybe create 
//            separate classes for these?
//  5) Fixme: in the member function isFlagBit set, the code with thrown an exception if the requested
//            bit is out of range.  Is this the behaviour we want?
//

#include "mu2e-artdaq-core/Data/EWT.hh"
#include <array>
#include <cstdint>
#include <iosfwd>

namespace mu2e {

struct EventHeader {

  constexpr static char spillMask  = 0x1;

  EventHeader(){}

  EventHeader( EWT ewt, uint16_t mode, uint16_t rfmTDC, uint8_t flags ):
    ewt(ewt), mode(mode), rfmTDC(rfmTDC), flags(flags) {
  }

  EWT        ewt   = 0;  // Event Window Tag
  uint32_t   mode  = 0;  // Event Mode
  uint8_t   rfmTDC = 0;  // RF Marker TDC
  uint8_t    flags = 0;  // on-spill bit and reserved flags

  bool isOnSpill() const{
    return ( (flags & spillMask) == 1);
  }

  bool isOffSpill() const {
    return ( ! isOnSpill() );
  }

  bool isFlagBitSet( int bit) const;

};

  std::ostream& operator<<(std::ostream& os,
			   EventHeader const& eh );

}
#endif /* mu2e_artdaq_core_Data_EventHeader_hh */
