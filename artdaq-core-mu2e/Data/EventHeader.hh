#ifndef mu2e_artdaq_core_Data_EventHeader_hh
#define mu2e_artdaq_core_Data_EventHeader_hh
//
// Header describing a Mu2e art::Event
//
// Notes:
//  1) See Mu2e-doc-4914 for the definition of the informaton in the Heartbeat packet.
//     See Mu2e-doc-19095, page 22 for the definition of the propose CFO Event Window Data Record.
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

#include "artdaq-core-mu2e/Data/EWT.hh"
#include <array>
#include <cstdint>
#include <iosfwd>

namespace mu2e {

struct EventHeader {

  constexpr static uint8_t spillMask  = 0x1;

  EventHeader(){}

  EventHeader( EWT ewt, uint32_t mode, uint8_t rfmTDC, uint8_t flags ): ewt(ewt), mode(mode), rfmTDC_est(rfmTDC), flags(flags),
    rnr_check(0), ndtc_check(0), dtc_check(0), ewt_check(0) {
  }

  EventHeader( EWT ewt, uint32_t mode, uint8_t rfmTDC, uint8_t flags,
	       uint8_t rnrCheck, uint8_t ndtcCheck, uint8_t dtcCheck, uint8_t ewtCheck): ewt(ewt), mode(mode), rfmTDC_est(rfmTDC), flags(flags),
    rnr_check(rnrCheck), ndtc_check(ndtcCheck), dtc_check(dtcCheck), ewt_check(ewtCheck) {
  }

  EventHeader( EWT ewt, uint32_t mode, uint8_t rfmTDC_est, uint8_t flags, uint16_t eventDuration, uint8_t rfmTDC_measured ):
    ewt(ewt), mode(mode), rfmTDC_est(rfmTDC_est), flags(flags), eventDuration(eventDuration), rfmTDC_measured(rfmTDC_measured),
    rnr_check(0), ndtc_check(0), dtc_check(0), ewt_check(0) {
  }

  EventHeader( EWT ewt, uint32_t mode, uint8_t rfmTDC_est, uint8_t flags, uint16_t eventDuration, uint8_t rfmTDC_measured,
               uint8_t rnrCheck, uint8_t ndtcCheck, uint8_t dtcCheck, uint8_t ewtCheck):
    ewt(ewt), mode(mode), rfmTDC_est(rfmTDC_est), flags(flags), eventDuration(eventDuration), rfmTDC_measured(rfmTDC_measured),
    rnr_check(rnrCheck), ndtc_check(ndtcCheck), dtc_check(dtcCheck), ewt_check(ewtCheck) {
  }

  // Information from the Heartbeat Packet
  EWT        ewt        = 0;  // Event Window Tag
  uint32_t   mode       = 0;  // Event Mode
  uint8_t    rfmTDC_est = 0;  // RF Marker TDC
  uint8_t    flags      = 0;  // on-spill bit and reserved flags

  // Information from the CFO Event Window Data Record
  uint16_t eventDuration   = 0;
  uint8_t  rfmTDC_measured = 0;

  uint8_t    rnr_check : 1; //Round-robin check
  uint8_t    ndtc_check : 1; //check if nDTCs used in the event matches the configured value
  uint8_t    dtc_check : 1; //Check we have all the DTCs used in the event
  uint8_t    ewt_check : 1; //eventHeader EWT consistency with the subEvents header

  bool isOnSpill() const{
    return ( (flags & spillMask) == 1);
  }

  bool isOffSpill() const {
    return ( ! isOnSpill() );
  }

  bool isFlagBitSet( int bit) const;

  void initErrorChecks(){
    rnr_check  = 1;
    dtc_check  = 1;
    ndtc_check = 1;
    ewt_check  = 1;
  }
};

  std::ostream& operator<<(std::ostream& os,
			   EventHeader const& eh );

}
#endif /* mu2e_artdaq_core_Data_EventHeader_hh */
