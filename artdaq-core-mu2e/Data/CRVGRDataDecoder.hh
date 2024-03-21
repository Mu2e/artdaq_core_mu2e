
#ifndef ARTDAQ_CORE_MU2E_DATA_CRVGRDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CRVGRDATADECODER_HH

#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"
#include <memory>
#include <vector>
#include <bitset>

namespace mu2e {
class CRVGRDataDecoder : public DTCDataDecoder
{
public:
	CRVGRDataDecoder()
		: DTCDataDecoder() {}

	CRVGRDataDecoder(std::vector<uint8_t> data)
		: DTCDataDecoder(data) {}

	explicit CRVGRDataDecoder(DTCLib::DTC_SubEvent const& f)
		: DTCDataDecoder(f)
	{}

	struct CRVGRStatusPacket
	{
		uint8_t CRCErrorCnt;
		uint8_t LosCnt : 4;  // == 0x06
                uint8_t unused : 3;
                uint8_t PLLlock : 1;
		uint16_t EventWindowTag0;
                uint16_t EventWindowTag1;
                uint16_t unused2 : 12;
                uint16_t BeamOn : 1;
                uint16_t unused3 : 3;
                uint16_t lastWindow;
                uint8_t unused4;
                uint8_t MarkerDelayCnt;
                uint8_t unused5;
                uint8_t HeartBeatCnt;
                uint16_t InjectionTs;
		CRVGRStatusPacket()
			: CRCErrorCnt(0)
			, LosCnt(0)
			, unused(0)
			, PLLlock(0)
			, EventWindowTag0(0)
			, EventWindowTag1(0)
			, unused2(0)
			, BeamOn(0)
			, unused3(0)
			, lastWindow(0)
			, unused4(0)
			, MarkerDelayCnt(0)
                        , unused5(0)
                        , HeartBeatCnt(0)
                        , InjectionTs(0)
		{}

                uint32_t GetEventWindowTag() const
                {
                  uint32_t EventWindowTag = EventWindowTag1;
                  EventWindowTag<<=16;
                  EventWindowTag|=EventWindowTag0;
                  return EventWindowTag;
                }
	};

	std::unique_ptr<CRVGRStatusPacket> GetCRVGRStatusPacket(size_t blockIndex) const;

};
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_CRVGRDATADECODER_HH
