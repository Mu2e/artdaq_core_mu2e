
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
        /* For FEB-I: soon to be obsolete                         */
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
        /* For FEB-II                                             */
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

	/// Raw hit structure that matches the raw DAQ data format
	/// this an be cast directly from raw data pointer avoiding any copying
	struct CRVHitRawFEBII
	{
		CRVHitInfoFEBII hitInfo;                     // (port, channel, time, etc.)
		CRVHitADCBlockFEBII adcBlocks[nADCblocks];  // 3 blocks, each with 4 x 12-bit packed samples for a total of 12 samples

		// Direct accessors for info fields
		uint16_t getPortNumber() const { return hitInfo.portNumber; }
		uint16_t getFpgaNumber() const { return hitInfo.fpgaNumber; }
		uint16_t getFpgaChannel() const { return hitInfo.fpgaChannel; }
		uint16_t getHitTime() const { return hitInfo.hitTime; }

		// Extract waveform by unpacking 12-bit samples (this copies data!)
		std::vector<int16_t> getWaveform() const
		{
			std::vector<int16_t> waveform;
			waveform.reserve(nADCsamples);
			for (size_t i = 0; i < nADCblocks; ++i)
			{
				waveform.push_back(static_cast<int16_t>(adcBlocks[i].getSample0()));
				waveform.push_back(static_cast<int16_t>(adcBlocks[i].getSample1()));
				waveform.push_back(static_cast<int16_t>(adcBlocks[i].getSample2()));
				waveform.push_back(static_cast<int16_t>(adcBlocks[i].getSample3()));
			}
			return waveform;
		}
	};

	constexpr static std::size_t hitSize = sizeof(CRVHitInfoFEBII) + nADCblocks * sizeof(CRVHitADCBlockFEBII);
	static_assert(sizeof(CRVHitRawFEBII) == hitSize,
	              "CRVHitRawFEBII size must match raw data layout");

	/// Range/view class for iterating over raw hits without copying
	/// Points to raw data in memory - no copying until getWaveform() is called
	/// When iterating with "for (const auto& hit : range)", hit is a reference to CRVHitRawFEBII
	/// which itself is just a view into raw memory - zero copying of hit data.
	class CRVHitRangeFEBII
	{
	public:
		// For consistency with STL convention
		using const_iterator = const CRVHitRawFEBII*;

		CRVHitRangeFEBII(const CRVHitRawFEBII* hits, size_t count)
			: hits_(hits), count_(count) {}

		// Iterator support for range-based for loops
		// Returns raw pointers as iterators (they satisfy all iterator requirements)
		const_iterator begin() const { return hits_; }
		const_iterator end() const { return hits_ + count_; }
		
		size_t size() const { return count_; }
		bool empty() const { return count_ == 0; }
		bool error() const { return hits_ == nullptr; } //e.g. if the hit payload is not a multiple of the hitsize
		                                                //count_==0 indicates no hits, but not necessarily corrupted data

		// Direct access by index - returns reference (no copy)
		const CRVHitRawFEBII& operator[](size_t index) const { return hits_[index]; }

	private:
		const CRVHitRawFEBII* hits_;
		size_t count_;
	};


	/**********************************************************/
	/* GlobalRun Info                                         */
	/**********************************************************/
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
	/* access functions (used for CrvDigis and GlobalRun)     */
	/**********************************************************/

	std::unique_ptr<CRVROCStatusPacket> GetCRVROCStatusPacket(size_t blockIndex) const;
	const CRVROCStatusPacketFEBII* GetCRVROCStatusPacketFEBII(size_t blockIndex) const;
	bool GetCRVHits(size_t blockIndex, std::vector<CRVHit> &crvHits) const;
	CRVHitRangeFEBII GetCRVHitRangeFEBII(size_t blockIndex) const;  // Returns range for zero-copy iteration
	void PrintBlockFEBII(size_t blockIndex) const;
	bool GetCRVGlobalRunInfo(size_t blockIndex, mu2e::CRVDataDecoder::CRVGlobalRunInfo &globalRunInfo) const;
	bool GetCRVGlobalRunPayload(size_t blockIndex, std::vector<uint16_t> &globalRunPayload) const;
};
using CRVDataDecoders = std::vector<CRVDataDecoder>;
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_CRVDATADECODER_HH
