
#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_CALORIMETERFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_CALORIMETERFRAGMENT_HH

#include "mu2e-artdaq-core/Overlays/ArtFragment.hh"

namespace mu2e {
class CalorimeterFragment : public ArtFragment
{
public:
	explicit CalorimeterFragment(artdaq::Fragment const& f)
		: ArtFragment(f) {}

	CalorimeterFragment(const void* ptr, size_t sz)
		: ArtFragment(ptr, sz) {}

	explicit CalorimeterFragment(std::pair<const void*, size_t> p)
		: CalorimeterFragment(p.first, p.second) {}

	struct CalorimeterDataPacket
	{
		uint16_t NumberOfHits;

		CalorimeterDataPacket()
			: NumberOfHits(0) {}
	};

	struct CalorimeterBoardID
	{
		uint16_t BoardID : 10;
		uint16_t ChannelStatusFlagsA : 6;
		uint16_t ChannelStatusFlagsB : 14;
		uint16_t unused : 2;

		CalorimeterBoardID()
			: BoardID(0), ChannelStatusFlagsA(0), ChannelStatusFlagsB(0), unused(0) {}
	};

	struct CalorimeterHitReadoutPacket
	{
		uint16_t ChannelNumber : 6;
		uint16_t DIRACA : 10;
		uint16_t DIRACB;
		uint16_t ErrorFlags;
		uint16_t Time;
		uint8_t NumberOfSamples;
		uint8_t IndexOfMaxDigitizerSample;

		CalorimeterHitReadoutPacket()
			: ChannelNumber(0), DIRACA(0), DIRACB(0), ErrorFlags(0), Time(0), NumberOfSamples(0), IndexOfMaxDigitizerSample(0) {}
	};

	std::unique_ptr<CalorimeterDataPacket> GetCalorimeterData(size_t blockIndex) const;
	std::unique_ptr<CalorimeterBoardID> GetCalorimeterBoardID(size_t blockIndex) const;
	std::vector<std::pair<CalorimeterHitReadoutPacket, std::vector<uint16_t>>> GetCalorimeterHits(size_t blockIndex) const;
	std::vector<std::pair<CalorimeterHitReadoutPacket, uint16_t>> GetCalorimeterHitsForTrigger(size_t blockIndex) const;
};
}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_CALORIMETERFRAGMENT_HH