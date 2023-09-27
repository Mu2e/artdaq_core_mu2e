
#ifndef ARTDAQ_CORE_MU2E_OVERLAYS_TRKDTCFRAGMENT_HH
#define ARTDAQ_CORE_MU2E_OVERLAYS_TRKDTCFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"

namespace mu2e {
class TrkDtcFragment
{
public:
	TrkDtcFragment(artdaq::Fragment const& f)
		: artdaq_fragment_(f) {}

	static constexpr int32_t CURRENT_VERSION = 1;

	struct Metadata
	{
		int32_t version;

		static size_t const size_words = 4;  ///< Size of the Metadata struct, in bytes
	};

	static Metadata create_metadata()
	{
		Metadata m;
		m.version = CURRENT_VERSION;
		return m;
	}

	struct RegEntry
	{
		uint32_t address;
		uint32_t value;
	};

	static_assert(sizeof(Metadata) == Metadata::size_words, "Metadata size changed!");

	int nReg() { return artdaq_fragment_.dataSizeBytes() / sizeof(RegEntry)); }

	int32_t version() { return artdaq_fragment_.metadata()->version; }

	RegEntry getRegisterEntry(size_t index)
	{
		if (index >= nReg())
		{
			TLOG(TLVL_ERROR, "TrkDtcFragment") << "Index " << index << " is out of range! (nReg=" << nReg() << ")";
			return RegEntry();
		}
		return *(reinterpret_cast<RegEntry*>(artdaq_fragment_.dataBeginBytes()) + index);
	}

	uint32_t reg(int index)
	{
		return getRegisterEntry(index).address;
	}
	uint32_t val(int index) { return getRegisterEntry(index).value; }

private:
	artdaq::Fragment const& artdaq_fragment_;
};
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_OVERLAYS_TRKDTCFRAGMENT_HH
