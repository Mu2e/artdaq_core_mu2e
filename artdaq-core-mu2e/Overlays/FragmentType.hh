#ifndef mu2e_artdaq_core_Overlays_FragmentType_hh
#define mu2e_artdaq_core_Overlays_FragmentType_hh
#include "artdaq-core/Data/Fragment.hh"

namespace mu2e {
static std::vector<std::string> const names{"MISSED", "DTC", "MU2E", "MU2EEVENT", "TRK", "CAL", "CRV", "DBG", "UNKNOWN"};

namespace detail {
enum FragmentType : artdaq::Fragment::type_t
{
	EMPTY = artdaq::Fragment::EmptyFragmentType,
	MISSED = artdaq::Fragment::FirstUserFragmentType,
	DTC,
	MU2E,
	MU2EEVENT,
	TRK,     // Tracker fragment
	CAL,     // Calorimeter fragment
	CRV,     // Cosmic Ray Veto fragment
	DBG,     // Debug Packet Fragment
	INVALID  // Should always be last.
};

// Safety check.
static_assert(artdaq::Fragment::isUserFragmentType(FragmentType::INVALID - 1), "Too many user-defined fragments!");
}  // namespace detail

using detail::FragmentType;

FragmentType toFragmentType(std::string t_string);
std::string fragmentTypeToString(FragmentType val);

/**
 * \brief Create a list of all Fragment types defined by this package, in the format that RawInput expects
 * \return A list of all Fragment types defined by this package, in the format that RawInput expects
 */
std::map<artdaq::Fragment::type_t, std::string> makeFragmentTypeMap();
}  // namespace mu2e
#endif /* mu2e_artdaq_Overlays_FragmentType_hh */
