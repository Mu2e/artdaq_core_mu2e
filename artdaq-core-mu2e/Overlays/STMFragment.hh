
#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core-mu2e/Overlays/ArtFragment.hh"

namespace mu2e {
class STMFragment : public ArtFragment
{
public:
	explicit STMFragment(artdaq::Fragment const& f)
		: ArtFragment(f) {}

	STMFragment(const void* ptr, size_t sz)
		: ArtFragment(ptr, sz) {}

	explicit STMFragment(std::pair<const void*, size_t> p)
		: STMFragment(p.first, p.second) {}

	struct STMDataPacket
	{
		uint16_t FixedHeaderWord[2];

		STMDataPacket() {}
	};
  /*
	struct STMBoardID
	{
		uint16_t BoardID : 10;
		uint16_t ChannelStatusFlagsA : 6;
		uint16_t ChannelStatusFlagsB : 14;
		uint16_t unused : 2;

		STMBoardID()
			: BoardID(0), ChannelStatusFlagsA(0), ChannelStatusFlagsB(0), unused(0) {}
	};

	struct STMHitReadoutPacket
	{
		uint16_t ChannelNumber : 6;
		uint16_t DIRACA : 10;
		uint16_t DIRACB;
		uint16_t ErrorFlags;
		uint16_t Time;
		uint8_t NumberOfSamples;
		uint8_t IndexOfMaxDigitizerSample;

		STMHitReadoutPacket()
			: ChannelNumber(0), DIRACA(0), DIRACB(0), ErrorFlags(0), Time(0), NumberOfSamples(0), IndexOfMaxDigitizerSample(0) {}
	};
*/
	std::unique_ptr<STMDataPacket> GetSTMData(size_t blockIndex) const;
  //	std::unique_ptr<STMBoardID> GetSTMBoardID(size_t blockIndex) const;
  //	std::vector<std::pair<STMHitReadoutPacket, std::vector<uint16_t>>> GetSTMHits(size_t blockIndex) const;
  //	std::vector<std::pair<STMHitReadoutPacket, uint16_t>> GetSTMHitsForTrigger(size_t blockIndex) const;
};

  std::ostream& operator<<(std::ostream& os, const STMFragment::STMDataPacket& data) {
    os << std::hex << data.FixedHeaderWord[0] << ", " << data.FixedHeaderWord[0] << std::endl;
    return os;
  }
}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
