#ifndef mu2e_artdaq_core_Overlays_EWT_hh
#define mu2e_artdaq_core_Overlays_EWT_hh
//
// A type representing an Event Window Tag.
//
// Notes:
//  1) See Mu2e-doc-4914 for the definition of the information in the Heartbeat packet.
//  2) Fixme: do we want to make this a class and persist only 48 bits?
//

#include <cstdint>

namespace mu2e {

  typedef uint64_t EWT;

}
#endif /* mu2e_artdaq_core_Overlays */
