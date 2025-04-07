#ifndef artdaq_core_Data_Mu2eEventFragment_hh
#define artdaq_core_Data_Mu2eEventFragment_hh

#include <memory>
#include "artdaq-core-mu2e/Overlays/CFO_Packets/CFO_Event.h"
#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

// #include <ostream>
// #include <vector>

// Implementation of "CFOEventFragment", an artdaq::Fragment overlay class

namespace mu2e {
class CFOEventFragment;
}

/**
 * \brief The artdaq::CFOEventFragment class represents a Fragment which contains one or more DTC_Events
 */
class mu2e::CFOEventFragment
{
public:
	/// The current version of the CFOEventFragment
	static constexpr uint8_t CURRENT_VERSION = 1;

	/**
	 * \param f The Fragment object to use for data storage
	 *
	 * The constructor simply sets its const private member "artdaq_Fragment_"
	 * to refer to the artdaq::Fragment object
	 */
	explicit CFOEventFragment(artdaq::Fragment const& f)
		: artdaq_Fragment_(f) {}

	virtual ~CFOEventFragment()
	{
	}

	CFOLib::CFO_Event getData() const
	{
		if (event_ptr_ == nullptr)
		{
			event_ptr_.reset(new CFOLib::CFO_Event(artdaq_Fragment_.dataBeginBytes()));
			//			event_ptr_->SetupEvent();
		}
		return *event_ptr_.get();
	}

protected:
private:
	CFOEventFragment(CFOEventFragment const&) = delete;             // CFOEventFragment should definitely not be copied
	CFOEventFragment(CFOEventFragment&&) = delete;                  // CFOEventFragment should not be moved, only the underlying Fragment
	CFOEventFragment& operator=(CFOEventFragment const&) = delete;  // CFOEventFragment should definitely not be copied
	CFOEventFragment& operator=(CFOEventFragment&&) = delete;       // CFOEventFragment should not be moved, only the underlying Fragment

	artdaq::Fragment const& artdaq_Fragment_;
	mutable std::unique_ptr<CFOLib::CFO_Event> event_ptr_{nullptr};
};

#endif /* artdaq_core_Data_Mu2eEventFragment_hh */
