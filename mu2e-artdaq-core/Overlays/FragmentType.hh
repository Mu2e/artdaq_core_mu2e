#ifndef mu2e_artdaq_core_Overlays_FragmentType_hh
#define mu2e_artdaq_core_Overlays_FragmentType_hh
#include "artdaq-core/Data/Fragment.hh"

namespace mu2e {

  namespace detail {
    enum FragmentType : artdaq::Fragment::type_t
      { EMPTY = artdaq::Fragment::EmptyFragmentType,
      MISSED = artdaq::Fragment::FirstUserFragmentType,
      DTC,
      MU2E,
	  TRK, // Tracker fragment
	  CAL, // Calorimeter fragment
	  CRV, // Cosmic Ray Veto fragment
	  DBG, // Debug Packet Fragment
      INVALID // Should always be last.
    };

    // Safety check.
    static_assert(artdaq::Fragment::isUserFragmentType(FragmentType::INVALID - 1),
                  "Too many user-defined fragments!");
  }

  using detail::FragmentType;

  FragmentType toFragmentType(std::string t_string);
  std::string fragmentTypeToString(FragmentType val);
}
#endif /* mu2e_artdaq_Overlays_FragmentType_hh */
