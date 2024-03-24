
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

    struct CRVGRRawPacket {
		uint16_t w0;
		uint16_t w1;
		uint16_t w2;
		uint16_t w3;
		uint16_t w4;
		uint16_t w5;
		uint16_t w6;
		uint16_t w7;
		CRVGRRawPacket()
		    : w0(0)
			, w1(0)
			, w2(0)
			, w3(0)
			, w4(0)
			, w5(0)
			, w6(0)
			, w7(0) 
			{}
	};
	std::unique_ptr<CRVGRRawPacket> GetCRVGRRawPacket(size_t blockIndex) const;

	struct CRVGRStatusPacket{
		
		uint16_t CRCErrorCnt : 8;
		uint16_t LossCnt : 4;  // == 0x06
        uint16_t unused : 3;
        uint16_t PLLlock : 1;
		uint16_t EventWindowTag0;
        uint16_t EventWindowTag1;
        uint16_t unused2 : 12;
        uint16_t BeamOn : 1;
        uint16_t unused3 : 3;
        uint16_t lastWindow;
        uint16_t unused4 : 8;
        uint16_t MarkerDelayCnt : 8;
        uint16_t unused5 :8 ;
        uint16_t HeartBeatCnt : 8;
        uint16_t InjectionTs;

		CRVGRStatusPacket()
			: CRCErrorCnt(0)
			, LossCnt(0)
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
        uint32_t GetEventWindowTag() const {
            uint32_t EventWindowTag = EventWindowTag1;
            EventWindowTag<<=16;
            EventWindowTag|=EventWindowTag0;
            return EventWindowTag;
        }
	};

	std::unique_ptr<CRVGRStatusPacket> GetCRVGRStatusPacket(size_t blockIndex) const;
};

std::ostream& operator<<(std::ostream& os,
			   CRVGRDataDecoder::CRVGRStatusPacket const& status );
std::ostream& operator<<(std::ostream& os,
			   CRVGRDataDecoder::CRVGRRawPacket const& status );

}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_CRVGRDATADECODER_HH
