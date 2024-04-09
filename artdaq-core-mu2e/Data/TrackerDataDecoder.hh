
#ifndef ARTDAQ_CORE_MU2E_DATA_TRACKERDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_TRACKERDATADECODER_HH

#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"

#include <messagefacility/MessageLogger/MessageLogger.h> // Putting this here so that Offline/DAQ/src/FragmentAna_module.cc can use it

#include <vector>

namespace mu2e {
class TrackerDataDecoder : public DTCDataDecoder
{
public:
	static constexpr int TRACKER_FORMAT_VERSION = 1;

	TrackerDataDecoder()
		: DTCDataDecoder() {}
	explicit TrackerDataDecoder(std::vector<uint8_t> data);

	explicit TrackerDataDecoder(DTCLib::DTC_SubEvent const& evt);

	struct TrackerDataPacketV0
	{
		uint16_t StrawIndex;

		uint16_t TDC0;

		uint16_t TDC1;

		uint16_t TOT0 : 8;
		uint16_t TOT1 : 8;

		uint16_t ADC00 : 12;
		uint16_t ADC01A : 4;

		uint16_t ADC01B : 8;
		uint16_t ADC02A : 8;

		uint16_t ADC02B : 4;
		uint16_t ADC03 : 12;

		uint16_t ADC04 : 12;
		uint16_t ADC05A : 4;

		uint16_t ADC05B : 8;
		uint16_t ADC06A : 8;

		uint16_t ADC06B : 4;
		uint16_t ADC07 : 12;

		uint16_t ADC08 : 12;
		uint16_t ADC09A : 4;

		uint16_t ADC09B : 8;
		uint16_t ADC10A : 8;

		uint16_t ADC10B : 4;
		uint16_t ADC11 : 12;

		uint16_t ADC12 : 12;
		uint16_t ADC13A : 4;

		uint16_t ADC13B : 8;
		uint16_t ADC14A : 8;

		uint16_t ADC14B : 4;
		uint16_t unused1 : 4;
		uint16_t PreprocessingFlags : 8;

		inline uint16_t ADC01() const { return ADC01A + (ADC01B << 4); }
		inline uint16_t ADC02() const { return ADC02A + (ADC02B << 8); }
		inline uint16_t ADC05() const { return ADC05A + (ADC05B << 4); }
		inline uint16_t ADC06() const { return ADC06A + (ADC06B << 8); }
		inline uint16_t ADC09() const { return ADC09A + (ADC09B << 4); }
		inline uint16_t ADC10() const { return ADC10A + (ADC10B << 8); }
		inline uint16_t ADC13() const { return ADC13A + (ADC13B << 4); }
		inline uint16_t ADC14() const { return ADC14A + (ADC14B << 8); }

		inline void SetWaveform(size_t index, uint16_t waveform)
		{
			switch (index)
			{
				case 0:
					ADC00 = waveform & 0xFFF;
					break;
				case 1:
					ADC01A = waveform & 0xF;
					ADC01B = (waveform >> 4) & 0xFF;
					break;
				case 2:
					ADC02A = waveform & 0xFF;
					ADC02B = (waveform >> 8) & 0xF;
					break;
				case 3:
					ADC03 = waveform & 0xFFF;
					break;
				case 4:
					ADC04 = waveform & 0xFFF;
					break;
				case 5:
					ADC05A = waveform & 0xF;
					ADC05B = (waveform >> 4) & 0xFF;
					break;
				case 6:
					ADC06A = waveform & 0xFF;
					ADC06B = (waveform >> 8) & 0xF;
					break;
				case 7:
					ADC07 = waveform & 0xFFF;
					break;
				case 8:
					ADC08 = waveform & 0xFFF;
					break;
				case 9:
					ADC09A = waveform & 0xF;
					ADC09B = (waveform >> 4) & 0xFF;
					break;
				case 10:
					ADC10A = waveform & 0xFF;
					ADC10B = (waveform >> 8) & 0xF;
					break;
				case 11:
					ADC11 = waveform & 0xFFF;
					break;
				case 12:
					ADC12 = waveform & 0xFFF;
					break;
				case 13:
					ADC13A = waveform & 0xF;
					ADC13B = (waveform >> 4) & 0xFF;
					break;
				case 14:
					ADC14A = waveform & 0xFF;
					ADC14B = (waveform >> 8) & 0xF;
					break;
			}
		}
	};

	struct TrackerDataPacket
	{
		uint16_t StrawIndex;

		uint16_t TDC0A;

		uint16_t TDC0B : 8;
		uint16_t TOT0 : 4;
		uint16_t EWMCounter : 4;

		uint16_t TDC1A;

		uint16_t TDC1B : 8;
		uint16_t TOT1 : 4;
		uint16_t ErrorFlags : 4;

		uint16_t NumADCPackets : 6;
		uint16_t PMP : 10;

		uint16_t ADC00 : 10;
		uint16_t ADC01A : 6;

		uint16_t ADC01B : 4;
		uint16_t ADC02 : 10;
		uint16_t unused1 : 2;

		inline uint32_t TDC0() const { return static_cast<uint32_t>((TDC0B << 16) + TDC0A); }
		inline uint32_t TDC1() const { return static_cast<uint32_t>((TDC1B << 16) + TDC1A); }
		inline uint16_t ADC01() const { return ADC01A + (ADC01B << 6); }

		inline void SetTDC0(uint32_t _tdc0)
		{
			TDC0A = _tdc0 & 0xFFFF;
			TDC0B = (_tdc0 >> 16) & 0xFF;
		}

		inline void SetTDC1(uint32_t _tdc1)
		{
			TDC1A = _tdc1 & 0xFFFF;
			TDC1B = (_tdc1 >> 16) & 0xFF;
		}

		inline void SetWaveform(size_t index, uint16_t waveform)
		{
			switch (index)
			{
				case 0:
					ADC00 = waveform & 0x3FF;
					break;
				case 1:
					ADC01A = waveform & 0x3F;
					ADC01B = (waveform >> 6) & 0xF;
					break;
				case 2:
					ADC02 = waveform & 0x3FF;
					break;
			}
		}
	};

	struct TrackerADCPacket
	{
		uint16_t ADC0 : 10;
		uint16_t ADC1A : 6;

		uint16_t ADC1B : 4;
		uint16_t ADC2 : 10;
		uint16_t unused0 : 2;

		uint16_t ADC3 : 10;
		uint16_t ADC4A : 6;

		uint16_t ADC4B : 4;
		uint16_t ADC5 : 10;
		uint16_t unused1 : 2;

		uint16_t ADC6 : 10;
		uint16_t ADC7A : 6;

		uint16_t ADC7B : 4;
		uint16_t ADC8 : 10;
		uint16_t unused2 : 2;

		uint16_t ADC9 : 10;
		uint16_t ADC10A : 6;

		uint16_t ADC10B : 4;
		uint16_t ADC11 : 10;
		uint16_t unused3 : 2;

		inline uint16_t ADC1() const { return ADC1A + (ADC1B << 6); }
		inline uint16_t ADC4() const { return ADC4A + (ADC4B << 6); }
		inline uint16_t ADC7() const { return ADC7A + (ADC7B << 6); }
		inline uint16_t ADC10() const { return ADC10A + (ADC10B << 6); }

		inline void SetWaveform(size_t index, uint16_t waveform)
		{
			switch (index)
			{
				case 0:
					ADC0 = waveform & 0x3FF;
					break;
				case 1:
					ADC1A = waveform & 0x3F;
					ADC1B = (waveform >> 6) & 0xF;
					break;
				case 2:
					ADC2 = waveform & 0x3FF;
					break;
				case 3:
					ADC3 = waveform & 0x3FF;
					break;
				case 4:
					ADC4A = waveform & 0x3F;
					ADC4B = (waveform >> 6) & 0xF;
					break;
				case 5:
					ADC5 = waveform & 0x3FF;
					break;
				case 6:
					ADC6 = waveform & 0x3FF;
					break;
				case 7:
					ADC7A = waveform & 0x3F;
					ADC7B = (waveform >> 6) & 0xF;
					break;
				case 8:
					ADC8 = waveform & 0x3FF;
					break;
				case 9:
					ADC9 = waveform & 0x3FF;
					break;
				case 10:
					ADC10A = waveform & 0x3F;
					ADC10B = (waveform >> 6) & 0xF;
					break;
				case 11:
					ADC11 = waveform & 0x3FF;
					break;
			}
		}
	};

	typedef std::vector<std::pair<const TrackerDataPacket*, std::vector<uint16_t>>> tracker_data_t;
	tracker_data_t GetTrackerData(size_t blockIndex, bool readWaveform = true) const;
	void ClearUpgradedPackets() { upgraded_data_packets_.clear(); }

private:
	const TrackerDataPacket* Upgrade(const TrackerDataPacketV0* input) const;
	std::vector<uint16_t> GetWaveformV0(const TrackerDataPacketV0* input) const;
	std::vector<uint16_t> GetWaveform(const TrackerDataPacket* input) const;

	mutable std::vector<TrackerDataPacket> upgraded_data_packets_;

};
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_TRACKERDATADECODER_HH
