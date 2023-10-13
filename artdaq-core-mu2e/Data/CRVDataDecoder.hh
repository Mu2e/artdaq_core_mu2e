
#ifndef ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH

#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"
#include <memory>
#include <vector>
#include <bitset>

namespace mu2e {
class CRVDataDecoder : public DTCDataDecoder
{
public:
	CRVDataDecoder()
		: DTCDataDecoder() {}

	CRVDataDecoder(std::vector<uint8_t> data)
		: DTCDataDecoder(data) {}

	explicit CRVDataDecoder(DTCLib::DTC_SubEvent const& f)
		: DTCDataDecoder(f)
	{}

	struct CRVROCStatusPacket
	{
		uint8_t unused1 : 4;
		uint8_t PacketType : 4;  // == 0x06
		uint8_t ControllerID;

		uint16_t ControllerEventWordCount;

		uint8_t ActiveFEBFlags2;
		uint8_t unused2;

		uint8_t ActiveFEBFlags0;
		uint8_t ActiveFEBFlags1;

		uint16_t TriggerCount;

		uint16_t MicroBunchStatus;

		uint16_t EventWindowTag1;

		uint16_t EventWindowTag0;

		CRVROCStatusPacket()
			: unused1(0)
			, PacketType(0)
			, ControllerID(0)
			, ControllerEventWordCount(0)
			, ActiveFEBFlags2(0)
			, unused2(0)
			, ActiveFEBFlags0(0)
			, ActiveFEBFlags1(0)
			, TriggerCount(0)
			, MicroBunchStatus(0)
			, EventWindowTag1(0)
			, EventWindowTag0(0)
		{}

                std::bitset<24> GetActiveFEBFlags() const
                {
                  uint32_t ActiveFEBFlags = ActiveFEBFlags2;
                  ActiveFEBFlags<<=8;
                  ActiveFEBFlags|=ActiveFEBFlags1;
                  ActiveFEBFlags<<=8;
                  ActiveFEBFlags|=ActiveFEBFlags0;
                  return std::bitset<24>(ActiveFEBFlags);  //only need the 24 lowest bits
                }

                uint32_t GetEventWindowTag() const
                {
                  uint32_t EventWindowTag = EventWindowTag1;
                  EventWindowTag<<=16;
                  EventWindowTag|=EventWindowTag0;
                  return EventWindowTag;
                }
	};

	struct CRVHitWaveformSample
	{
		int16_t ADC : 12;
		int16_t unused : 4;
		CRVHitWaveformSample()
			: ADC(0)
			, unused(0)
		{}
	};

	struct CRVHitInfo
	{
		uint16_t febChannel : 6;
		uint16_t portNumber : 5;
		uint16_t controllerNumber : 5;

		uint16_t HitTime : 12;
		uint16_t NumSamples : 4;

		CRVHitInfo()
			: febChannel(0)
			, portNumber(0)
			, controllerNumber(0)
			, HitTime(0)
			, NumSamples(0)
                {}
	};

        typedef std::vector<CRVHitWaveformSample> CRVHitWaveform;
        typedef std::pair<CRVHitInfo,CRVHitWaveform> CRVHit;

	std::unique_ptr<CRVROCStatusPacket> GetCRVROCStatusPacket(size_t blockIndex) const;
	std::vector<CRVHit> GetCRVHits(size_t blockIndex) const;

};
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH
