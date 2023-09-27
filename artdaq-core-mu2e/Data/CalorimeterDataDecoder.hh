
#ifndef ARTDAQ_CORE_MU2E_DATA_CALORIMETERDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CALORIMETERDATADECODER_HH

#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"

namespace mu2e {
class CalorimeterDataDecoder : public DTCDataDecoder
{
public:

	CalorimeterDataDecoder() : DTCDataDecoder() {}

	CalorimeterDataDecoder(std::vector<uint8_t> data)
		: DTCDataDecoder(data) {}

	explicit CalorimeterDataDecoder(DTCLib::DTC_SubEvent const& f)
		: DTCDataDecoder(f) {}


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

#endif  // ARTDAQ_CORE_MU2E_DATA_CALORIMETERDATADECODER_HH
