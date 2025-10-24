
#ifndef ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH

#include "artdaq-core-mu2e/Overlays/Decoders/DTCDataDecoder.hh"
#include <memory>
#include <vector>
#include <bitset>

namespace mu2e {
class CRVDataDecoder : public DTCDataDecoder
{
public:
	explicit CRVDataDecoder(DTCLib::DTC_SubEvent const &f)
		: DTCDataDecoder(f)
	{}

	/**********************************************************/

	// ROC Status Header (used for CrvDigis and global run)
	// see doc-db 4914
	struct CRVROCStatusPacket
	{
		uint8_t ControllerID : 4;
		uint8_t PacketType : 4;  // == 0x06
		uint8_t unused1;

		uint16_t ControllerEventWordCount;

		uint8_t ActiveFEBFlags2;
		uint8_t unused2;

		uint8_t ActiveFEBFlags0;
		uint8_t ActiveFEBFlags1;

		uint16_t TriggerCount;

		uint16_t MicroBunchStatus;  // EventWindowTag0 for global run

		uint16_t EventWindowTag1;  // EventWindowTag1 for global run

		uint16_t EventWindowTag0;  // EventWindowTag2 for global run

		CRVROCStatusPacket()
			: ControllerID(0)
			, PacketType(0)
			, unused1(0)
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
			ActiveFEBFlags <<= 8;
			ActiveFEBFlags |= ActiveFEBFlags1;
			ActiveFEBFlags <<= 8;
			ActiveFEBFlags |= ActiveFEBFlags0;
			return std::bitset<24>(ActiveFEBFlags);  // only need the 24 lowest bits
		}

		uint32_t GetEventWindowTag() const
		{
			uint32_t EventWindowTag = EventWindowTag1;
			EventWindowTag <<= 16;
			EventWindowTag |= EventWindowTag0;
			return EventWindowTag;
		}

		uint64_t GetEventWindowTagGlobalRun() const
		{
			uint64_t EventWindowTag = EventWindowTag0;
			EventWindowTag <<= 16;
			EventWindowTag |= EventWindowTag1;
			EventWindowTag <<= 16;
			EventWindowTag |= MicroBunchStatus;
			return EventWindowTag;
		}
	};

	/**********************************************************/

	// Hits (used for CrvDigi)
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
	typedef std::pair<CRVHitInfo, CRVHitWaveform> CRVHit;

	/**********************************************************/

	// ROC Status Header for FEB-II
	struct CRVROCStatusPacketFEBII
	{
		uint16_t ControllerEventWordCount;

		uint16_t TriggerCount;

		uint8_t ActiveFEBFlags2;
		uint8_t unused2;

		uint8_t ActiveFEBFlags0;
		uint8_t ActiveFEBFlags1;

		uint16_t MicroBunchStatus1;

		uint16_t MicroBunchStatus0;

		uint16_t EventWindowTag1;

		uint16_t EventWindowTag0;

		CRVROCStatusPacketFEBII()
			: ControllerEventWordCount(0)
			, TriggerCount(0)
			, ActiveFEBFlags2(0)
			, unused2(0)
			, ActiveFEBFlags0(0)
			, ActiveFEBFlags1(0)
			, MicroBunchStatus1(0)
			, MicroBunchStatus0(0)
			, EventWindowTag1(0)
			, EventWindowTag0(0)
		{}

		std::bitset<24> GetActiveFEBFlags() const
		{
			uint32_t ActiveFEBFlags = ActiveFEBFlags2;
			ActiveFEBFlags <<= 8;
			ActiveFEBFlags |= ActiveFEBFlags1;
			ActiveFEBFlags <<= 8;
			ActiveFEBFlags |= ActiveFEBFlags0;
			return std::bitset<24>(ActiveFEBFlags);  // only need the 24 lowest bits
		}

		uint32_t GetMicroBunchStatus() const
		{
			uint32_t MicroBunchStatus = MicroBunchStatus1;
			MicroBunchStatus <<= 16;
			MicroBunchStatus |= MicroBunchStatus0;
			return MicroBunchStatus;
		}

		uint32_t GetEventWindowTag() const
		{
			uint32_t EventWindowTag = EventWindowTag1;
			EventWindowTag <<= 16;
			EventWindowTag |= EventWindowTag0;
			return EventWindowTag;
		}
	};

	/**********************************************************/

	// Hit meta data for FEB-II
	struct CRVHitInfoFEBII
	{
		uint16_t fpgaChannel : 5;  // real channels: 0x00...0x0F (5th bit set to 0). special situation, e.g. fake pulses: 0x10...0x1F (5th bit set to 1)
		uint16_t fpgaNumber : 2;
		uint16_t portNumber : 5;
		uint16_t reserved : 4;
		uint16_t hitTime;
		CRVHitInfoFEBII()
			: fpgaChannel(0)
			, fpgaNumber(0)
			, portNumber(0)
			, reserved(0)
			, hitTime(0)
		{}
	};

	// ADC samples use 12 bit and are densily packed, i.e. 4 samples in a 3 word block (=ADC block below)
	constexpr static std::size_t nADCsamplesPerBlock = 4;
	constexpr static std::size_t nADCblocks = 3;    // each hit has 3 of such ADC blocks (with 4 ADC samples each)
	constexpr static std::size_t nADCsamples = 12;  // total number of ADC samples per hit
	struct CRVHitADCBlockFEBII
	{
		// first word
		uint16_t ADCsample0 : 12;
		uint16_t ADCsample1a : 4;
		// second word
		uint16_t ADCsample1b : 8;
		uint16_t ADCsample2a : 8;
		// third word
		uint16_t ADCsample2b : 4;
		uint16_t ADCsample3 : 12;
		CRVHitADCBlockFEBII()
			: ADCsample0(0)
			, ADCsample1a(0)
			, ADCsample1b(0)
			, ADCsample2a(0)
			, ADCsample2b(0)
			, ADCsample3(0)
		{}
		uint16_t getSample0() const { return ADCsample0; }
		uint16_t getSample1() const
		{
			uint16_t sample = ADCsample1b;
			sample <<= 4;
			sample |= ADCsample1a;
			return sample;
		}
		uint16_t getSample2() const
		{
			uint16_t sample = ADCsample2b;
			sample <<= 8;
			sample |= ADCsample2a;
			return sample;
		}
		uint16_t getSample3() const { return ADCsample3; }
	};

	constexpr static std::size_t hitSize = sizeof(CRVHitInfoFEBII) + nADCblocks * sizeof(CRVHitADCBlockFEBII);
	typedef std::vector<int16_t> CRVHitWaveformFEBII;  // ADC samples use only 12 bits, but are reported as 16 bits
	typedef std::pair<CRVHitInfoFEBII, CRVHitWaveformFEBII> CRVHitFEBII;

	/**********************************************************/

	// GlobalRun Info
	struct CRVGlobalRunInfo
	{
		uint16_t word0;
		uint16_t EWTCount;
		uint16_t markerCount;
		uint16_t lastEWT;

		uint16_t lock : 1;
		uint16_t unused : 3;
		uint16_t PLL : 4;
		uint16_t CRC : 8;

		uint16_t injectionWindow;
		uint16_t injectionTime;
		uint16_t word7;

		CRVGlobalRunInfo()
			: word0(0)
			, EWTCount(0)
			, markerCount(0)
			, lastEWT(0)
			, lock(0)
			, unused(0)
			, PLL(0)
			, CRC(0)
			, injectionWindow(0)
			, injectionTime(0)
			, word7(0)
		{}
	};

	// GlobalRun Payload
	typedef std::vector<uint16_t> CRVGlobalRunPayload;

	// Full GlobalRun Data
	struct CRVGlobalRunData
	{
		CRVROCStatusPacket _ROCstatus;
		CRVGlobalRunInfo _globalRunInfo;
		CRVGlobalRunPayload _globalRunPayload;

		CRVGlobalRunData()
			: _ROCstatus(), _globalRunInfo(), _globalRunPayload() {}

		CRVGlobalRunData(const CRVROCStatusPacket &ROCstatus, const CRVGlobalRunInfo &globalRunInfo, const CRVGlobalRunPayload &globalRunPayload)
			: _ROCstatus(ROCstatus), _globalRunInfo(globalRunInfo), _globalRunPayload(globalRunPayload) {}
	};
	typedef std::vector<CRVGlobalRunData> CRVGlobalRunDataCollection;

	/**********************************************************/

	// access functions (used for CrvDigis and GlobalRun)

	std::unique_ptr<CRVROCStatusPacket> GetCRVROCStatusPacket(size_t blockIndex) const;
	std::unique_ptr<CRVROCStatusPacketFEBII> GetCRVROCStatusPacketFEBII(size_t blockIndex) const;
	bool GetCRVHits(size_t blockIndex, std::vector<CRVHit> &crvHits) const;
	bool GetCRVHitsFEBII(size_t blockIndex, std::vector<CRVHitFEBII> &crvHits) const;
	bool GetCRVGlobalRunInfo(size_t blockIndex, mu2e::CRVDataDecoder::CRVGlobalRunInfo &globalRunInfo) const;
	bool GetCRVGlobalRunPayload(size_t blockIndex, std::vector<uint16_t> &globalRunPayload) const;
};
using CRVDataDecoders = std::vector<CRVDataDecoder>;
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH
