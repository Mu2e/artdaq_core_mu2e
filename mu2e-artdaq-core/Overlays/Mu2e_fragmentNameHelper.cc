#include "mu2e-artdaq-core/Overlays/FragmentType.hh"
#include "artdaq-core/Data/FragmentNameHelper.h"

#include "TRACE/tracemf.h"
#define TRACE_NAME "Mu2eFragmentNameHelper"


namespace mu2e {
/**
 * \brief Mu2eFragmentNameHelper extends ArtdaqFragmentNameHelper.
 * This implementation uses mu2e-artdaq-core's FragmentTypeMap and directly assigns names based on it
 */
class Mu2eFragmentNameHelper : public ArtdaqFragmentNameHelper
{
public:
	/**
	 * \brief Default Mu2eFragmentNameHelper Destructor
	 */
	virtual ~Mu2eFragmentNameHelper() = default;

	/**
	 * \brief Mu2eFragmentNameHelper Constructor
	 */
  Mu2eFragmentNameHelper(std::string unidentified_instance_name, std::vector<std::pair<artdaq::Fragment::type_t, std::string>> extraTypes);

private:
        Mu2eFragmentNameHelper(Mu2eFragmentNameHelper const&) = delete;

        Mu2eFragmentNameHelper(Mu2eFragmentNameHelper&&) = delete;

        Mu2eFragmentNameHelper& operator=(Mu2eFragmentNameHelper const&) = delete;

        Mu2eFragmentNameHelper& operator=(Mu2eFragmentNameHelper&&) = delete;

};

Mu2eArtdaqFragmentNameHelper::Mu2eArtdaqFragmentNameHelper(std::string unidentified_instance_name, std::vector<std::pair<artdaq::Fragment::type_t, std::string>> extraTypes)
  : ArtdaqFragmentNameHelper(unidentified_instance_name, extraTypes)
{
	TLOG(TLVL_DEBUG) << "Mu2eArtdaqFragmentNameHelper CONSTRUCTOR START";
	SetBasicTypes(mu2e::makeFragmentTypeMap());
	TLOG(TLVL_DEBUG) << "Mu2eArtdaqFragmentNameHelper CONSTRUCTOR END";
}

} // namespace mu2e

DEFINE_ARTDAQ_FRAGMENT_NAME_HELPER(mu2e::Mu2eFragmentNameHelper);
