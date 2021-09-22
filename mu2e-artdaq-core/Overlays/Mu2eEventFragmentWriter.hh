#ifndef artdaq_core_Data_Mu2eEventFragmentWriter_hh
#define artdaq_core_Data_Mu2eEventFragmentWriter_hh

////////////////////////////////////////////////////////////////////////
// Mu2eEventFragmentWriter
//
// This class gives write access to a Mu2eEventFragment. It should be
// used when multiple fragments are generated by one BoardReader for a
// single event.
//
////////////////////////////////////////////////////////////////////////

#include "mu2e-artdaq-core/Overlays/Mu2eEventFragment.hh"
#include "dtcInterfaceLib/DTC_Packets.h"
#include "artdaq-core/Data/Fragment.hh"

#include "tracemf.h"

#include <iostream>

namespace mu2e {
class Mu2eEventFragmentWriter;
}

/**
 * \brief A Read-Write version of the Mu2eEventFragment, used for filling Mu2eEventFragment objects with other Fragment objects
 */
class mu2e::Mu2eEventFragmentWriter : public mu2e::Mu2eEventFragment
{
public:
	/**
	 * \brief Constructs the Mu2eEventFragmentWriter
	 * \param f A Fragment object containing a Fragment header.
	 * \param expectedFragmentType The type of fragment which will be put into this Mu2eEventFragment
	 * \exception cet::exception if the Fragment input has inconsistent Header information
	 */
	explicit Mu2eEventFragmentWriter(artdaq::Fragment& f, uint64_t eventWindowTag, uint8_t evbMode);

	// ReSharper disable once CppMemberFunctionMayBeConst
	/**
	 * \brief Get the Mu2eEventFragment metadata (includes information about the location of Fragment objects within the Mu2eEventFragment)
	 * \return The Mu2eEventFragment metadata
	 */
	Metadata* metadata()
	{
		assert(artdaq_Fragment_.hasMetadata());
		return reinterpret_cast<Metadata*>(&*artdaq_Fragment_.metadataAddress());  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	}

	/**
	 * \brief Sets the missing_data flag
	 * \param isDataMissing The value of the missing_data flag
	 *
	 * The Mu2eEventFragment::Metadata::missing_data flag is used for FragmentGenerators to indicate that the fragment is incomplete,
	 * but the generator does not have the correct data to fill it. This happens for Window-mode FragmentGenerators when the window
	 * requested is before the start of the FragmentGenerator's buffers, for example.
	 */
	void set_missing_data(bool isDataMissing)
	{
		metadata()->missing_data = isDataMissing;
	}

	void fill_event(std::vector<std::unique_ptr<DTCLib::DTC_Event>> const& data, artdaq::Fragment::timestamp_t fragment_timestamp);

private:
	// Note that this non-const reference hides the const reference in the base class
	artdaq::Fragment& artdaq_Fragment_;

	void addSpace_(size_t bytes);

	uint8_t* dataBegin_() { return reinterpret_cast<uint8_t*>(&*artdaq_Fragment_.dataBegin()); }  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	void* dataEnd_() { return static_cast<void*>(dataBegin_() + lastFragmentIndex()); }           // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
};

#endif /* artdaq_core_Data_Mu2eEventFragmentWriter_hh */