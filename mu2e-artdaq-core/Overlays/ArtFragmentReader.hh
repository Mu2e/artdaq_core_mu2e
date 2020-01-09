#ifndef mu2e_artdaq_core_Overlays_ArtFragmentReader_hh
#define mu2e_artdaq_core_Overlays_ArtFragmentReader_hh

////////////////////////////////////////////////////////////////////////
// ArtFragmentReader
//
// Class derived from mu2eFragment which provides additional methods
// for accessing data stored in a mu2eFragment
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "mu2e-artdaq-core/Overlays/ArtFragment.hh"
//#include "RecoDataProducts/CrvDigi.hh"

#include <iostream>
#include <deque>
#include <unordered_map>

namespace mu2e {
class ArtFragmentReader;

std::ostream &operator<<(std::ostream &, ArtFragmentReader const &);
}  // namespace mu2e

class mu2e::ArtFragmentReader : public mu2e::ArtFragment
{
public:
	ArtFragmentReader(artdaq::Fragment const &f)
		: ArtFragment(f){};

	enum class PacketType : uint8_t
	{
		DCSRequest = 0,
		Heartbeat = 1,
		DataRequest = 2,
		DCSReply = 3,
		Dataheader = 5
	};

	const int format_version = 6;
	/**************************************************************************
	 ***************          DATA STRUCTURES                   ***************
	 **************************************************************************/
	struct DataBlockHeader
	{
		// Word 0
		uint16_t ByteCount;
		// Word 1
		uint8_t Hopcount : 4;
		uint8_t PacketType : 4;
		uint8_t ROCID : 3;
		uint8_t unused1 : 1;
		uint8_t SubsystemID : 3;
		uint8_t Valid : 1;
		// Word 2
		uint16_t PacketCount : 11;
		uint16_t unused2 : 5;
		// Word 3
		uint16_t TimestampLow;
		// Word 4
		uint16_t TimestampMed;
		// Word 5
		uint16_t TimestampHigh;
		// Word 6
		uint8_t Status;
		uint8_t FormatVersion;
		// Word 7
		uint8_t DTCID;
		uint8_t EVBMode;

		uint64_t GetTimestamp() const { return static_cast<uint64_t>(TimestampLow) + (static_cast<uint64_t>(TimestampMed) << 16) + (static_cast<uint64_t>(TimestampHigh) << 32); }
	};

	struct TrackerDataPacket
	{
		uint16_t StrawIndex;
		uint16_t TDC0;
		uint16_t TDC1;
		uint8_t TOT0;
		uint8_t TOT1;
		uint16_t ADC00 : 12;
		uint16_t ADC01A : 4;
		uint16_t ADC01B : 8;
		uint16_t ADC01() const { return ADC01A + (ADC01B << 4); }
		uint16_t ADC02A : 8;
		uint16_t ADC02B : 4;
		uint16_t ADC02() const { return ADC02A + (ADC02B << 8); }
		uint16_t ADC03 : 12;

		uint16_t ADC04 : 12;
		uint16_t ADC05A : 4;
		uint16_t ADC05B : 8;
		uint16_t ADC05() const { return ADC05A + (ADC05B << 4); }
		uint16_t ADC06A : 8;
		uint16_t ADC06B : 4;
		uint16_t ADC06() const { return ADC06A + (ADC06B << 8); }
		uint16_t ADC07 : 12;

		uint16_t ADC08 : 12;
		uint16_t ADC09A : 4;
		uint16_t ADC09B : 8;
		uint16_t ADC09() const { return ADC09A + (ADC09B << 4); }
		uint16_t ADC10A : 8;
		uint16_t ADC10B : 4;
		uint16_t ADC10() const { return ADC10A + (ADC10B << 8); }
		uint16_t ADC11 : 12;

		uint16_t ADC12 : 12;
		uint16_t ADC13A : 4;
		uint16_t ADC13B : 8;
		uint16_t ADC13() const { return ADC13A + (ADC13B << 4); }
		uint16_t ADC14A : 8;
		uint16_t ADC14B : 4;
		uint16_t ADC14() const { return ADC14A + (ADC14B << 8); }

		uint16_t unused1 : 4;
		uint16_t PreprocessingFlags : 8;

		std::array<adc_t, 15> Waveform() const
		{
			std::array<adc_t, 15> output;
			output[0] = ADC00;
			output[1] = ADC01();
			output[2] = ADC02();
			output[3] = ADC03;
			output[4] = ADC04;
			output[5] = ADC05();
			output[6] = ADC06();
			output[7] = ADC07;
			output[8] = ADC08;
			output[9] = ADC09();
			output[10] = ADC10();
			output[11] = ADC11;
			output[12] = ADC12;
			output[13] = ADC13();
			output[14] = ADC14();
			return output;
		}

		void SetWaveform(size_t index, adc_t waveform)
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

	struct CalorimeterDataPacket
	{
		uint16_t NumberOfHits;
	};

	struct CalorimeterBoardID
	{
		uint16_t BoardID : 10;
		uint16_t ChannelStatusFlagsA : 6;
		uint16_t ChannelStatusFlagsB : 14;
		uint16_t unused : 2;
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
	};

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
		uint8_t unused3;
		uint8_t unused4;
		uint16_t TriggerCount;
		uint8_t unused5;
		uint8_t unused6;
		uint8_t Errors;
		uint8_t EventType;
	};

	struct CRVHitReadoutPacket
	{
		uint16_t SiPMID;
		uint16_t HitTime : 10;
		uint16_t NumSamples : 6;
		uint8_t WaveformSample0;
		uint8_t WaveformSample1;
		uint8_t WaveformSample2;
		uint8_t WaveformSample3;
		uint8_t WaveformSample4;
		uint8_t WaveformSample5;
		uint8_t WaveformSample6;
		uint8_t WaveformSample7;

		std::array<unsigned int, 8> Waveform() const
		{
			std::array<unsigned int, 8> output;
			output[0] = WaveformSample0;
			output[1] = WaveformSample1;
			output[2] = WaveformSample2;
			output[3] = WaveformSample3;
			output[4] = WaveformSample4;
			output[5] = WaveformSample5;
			output[6] = WaveformSample6;
			output[7] = WaveformSample7;
			return output;
		}
	};

	/**************************************************************************
	 ***************         RETRIEVAL METHODS                  ***************
	 **************************************************************************/

	const DataBlockHeader *GetHeader(size_t block_num)
	{
		if (block_num >= block_count())
		{
			TLOG(TLVL_ERROR) << "Requested block " << block_num << " is outside the allowed range! (" << block_count() << ")";
			return nullptr;
		}
		if (blockSizeBytes(block_num) < sizeof(DataBlockHeader))
		{
			TLOG(TLVL_ERROR) << "Data block size indicates that it does not contain a complete DataBlockHeader! This data is probably corrupt!";
			return nullptr;
		}
		return reinterpret_cast<const DataBlockHeader *>(dataAtBlockIndex(block_num));
	}

	const TrackerDataPacket *GetTrackerData(size_t block_num)
	{
		auto hdr = GetHeader(block_num);
		if (hdr == nullptr) return nullptr;
		if (hdr->SubsystemID != 0)
		{
			TLOG(TLVL_ERROR) << "Trying to get Tracker data packet from non-Tracker DataBlock!";
			return nullptr;
		}
		if (blockSizeBytes(block_num) < sizeof(DataBlockHeader) + sizeof(TrackerDataPacket))
		{
			TLOG(TLVL_ERROR) << "Data Block size indicates that Tracker Data is not present! This event is probably corrupt!";
			return nullptr;
		}
		return reinterpret_cast<const TrackerDataPacket *>(hdr + 1);
	}

	const CalorimeterDataPacket *GetCalorimeterData(size_t block_num)
	{
		auto hdr = GetHeader(block_num);
		if (hdr == nullptr) return nullptr;
		if (hdr->SubsystemID != 1)
		{
			TLOG(TLVL_ERROR) << "Trying to get Calorimeter data packet from non-Calorimeter DataBlock!";
			return nullptr;
		}
		auto pkt = reinterpret_cast<const CalorimeterDataPacket *>(hdr + 1);

		if (blockSizeBytes(block_num) < sizeof(DataBlockHeader) + sizeof(CalorimeterDataPacket) + sizeof(CalorimeterBoardID) + sizeof(uint16_t) * (pkt->NumberOfHits + sizeof(CalorimeterHitReadoutPacket)))
		{
			TLOG(TLVL_ERROR) << "DataBlock size and Calorimeter header indicate that this DataBlock does not contain a complete Calorimeter event! This data is probably corrupt!";
			return nullptr;
		}

		return pkt;
	}

	const uint16_t *GetCalorimeterHitIndex(size_t block_num, size_t hit_num)
	{
		auto data_pkt = GetCalorimeterData(block_num);
		if (data_pkt == nullptr) return nullptr;
		if (hit_num >= data_pkt->NumberOfHits)
		{
			TLOG(TLVL_ERROR) << "Requested hit index " << hit_num << " is greater than the maximum (" << (data_pkt->NumberOfHits - 1) << ")!";
			return nullptr;
		}

		return reinterpret_cast<const uint16_t *>(data_pkt + 1) + hit_num;
	}

	const CalorimeterBoardID *GetCalorimeterBoardID(size_t block_num)
	{
		auto data_pkt = GetCalorimeterData(block_num);
		if (data_pkt == nullptr) return nullptr;

		return reinterpret_cast<const CalorimeterBoardID *>(reinterpret_cast<const uint16_t *>(data_pkt + 1) + data_pkt->NumberOfHits);
	}

	const CalorimeterHitReadoutPacket *GetCalorimeterReadoutPacket(size_t block_num, size_t hit_num)
	{
		auto end_ptr = reinterpret_cast<const uint8_t *>(dataAtBlockIndex(block_num)) + blockSizeBytes(block_num);

		auto data_pkt = GetCalorimeterData(block_num);
		if (data_pkt == nullptr) return nullptr;
		if (hit_num >= data_pkt->NumberOfHits)
		{
			TLOG(TLVL_ERROR) << "Requested Hit Readout Packet at index " << hit_num << ", but maximum index is " << (data_pkt->NumberOfHits - 1) << "!";
			return nullptr;
		}

		if (ReadoutPacketPointerCache[block_num].size() == 0)
		{
			auto ptr = reinterpret_cast<const CalorimeterHitReadoutPacket *>(GetCalorimeterBoardID(block_num) + 1);
			if (ptr == nullptr) return nullptr;
			if (reinterpret_cast<const uint8_t *>(ptr) > end_ptr)
			{
				TLOG(TLVL_ERROR) << "Fell off the end of the DataBlock while processing CalorimeterHitReadoutPackets!";
				return nullptr;
			}
			ReadoutPacketPointerCache[block_num].push_back(ptr);
		}

		while (ReadoutPacketPointerCache[block_num].size() <= hit_num)
		{
			auto last_ptr = ReadoutPacketPointerCache[block_num].back();
			auto ptr = reinterpret_cast<const CalorimeterHitReadoutPacket *>(reinterpret_cast<const uint16_t *>(last_ptr + 1) + last_ptr->NumberOfSamples);
			if (reinterpret_cast<const uint8_t *>(ptr) > end_ptr)
			{
				TLOG(TLVL_ERROR) << "Fell off the end of the DataBlock while processing CalorimeterHitReadoutPackets!";
				return nullptr;
			}
			ReadoutPacketPointerCache[block_num].push_back(ptr);
		}
		return ReadoutPacketPointerCache[block_num][hit_num];
	}

	const uint16_t *GetCalorimeterReadoutSample(size_t block_num, size_t hit_num, size_t sample_num)
	{
		auto pkt = GetCalorimeterReadoutPacket(block_num, hit_num);
		if (pkt == nullptr) return nullptr;
		if (sample_num >= pkt->NumberOfSamples)
		{
			TLOG(TLVL_ERROR) << "Requested sample index " << sample_num << " is greater than the maximum allowed (" << (pkt->NumberOfSamples - 1) << ")!";
			return nullptr;
		}
		return reinterpret_cast<const uint16_t *>(pkt + 1) + sample_num;
	}

	const CRVROCStatusPacket *GetCRVROCStatusPacket(size_t block_num)
	{
		auto hdr = GetHeader(block_num);
		if (hdr == nullptr) return nullptr;
		if (hdr->SubsystemID != 2)
		{
			TLOG(TLVL_ERROR) << "Trying to get CRV data packet from non-CRV DataBlock!";
			return nullptr;
		}

		if (blockSizeBytes(block_num) < sizeof(DataBlockHeader) + sizeof(CRVROCStatusPacket))
		{
			TLOG(TLVL_ERROR) << "Data Block size indicates that CRV ROC Status Packet is not present! This event is probably corrupt!";
			return nullptr;
		}

		auto crv_hdr = reinterpret_cast<const CRVROCStatusPacket *>(hdr + 1);
		if (blockSizeBytes(block_num) < sizeof(DataBlockHeader) + crv_hdr->ControllerEventWordCount)
		{
			TLOG(TLVL_ERROR) << "Inconsistency detected between Data Block Size (" << blockSizeBytes(block_num) << ") and CRV header's declared size (" << (sizeof(DataBlockHeader) + crv_hdr->ControllerEventWordCount) << ")! This event is probably corrupt!";
			return nullptr;
		}

		return crv_hdr;
	}

	size_t GetCRVHitCount(size_t block_num)
	{
		auto hdr = GetCRVROCStatusPacket(block_num);
		if (hdr == nullptr) return 0;

		size_t hit_bytes = hdr->ControllerEventWordCount - sizeof(CRVROCStatusPacket);

		size_t packet_count = hit_bytes / sizeof(CRVHitReadoutPacket);

		auto hit_packet = reinterpret_cast<const CRVHitReadoutPacket *>(hdr + 1);
		if (*reinterpret_cast<const uint8_t *>(hit_packet + (packet_count - 1)) == 0)
		{
			packet_count--;
		}

		return packet_count;
	}

	const CRVHitReadoutPacket *GetCRVHitReadoutPacket(size_t block_num, size_t hit_idx)
	{
		auto hdr = GetCRVROCStatusPacket(block_num);
		if (hdr == nullptr) return nullptr;

		auto hit_count = GetCRVHitCount(block_num);
		if (hit_idx >= hit_count)
		{
			TLOG(TLVL_ERROR) << "Requested hit index " << hit_idx << " is greater than the last hit index in this DataBlock (" << (hit_count - 1) << ")!";
			return nullptr;
		}

		return reinterpret_cast<const CRVHitReadoutPacket *>(hdr + 1) + hit_idx;
	}

private:
	std::unordered_map<size_t, std::deque<const CalorimeterHitReadoutPacket *>> ReadoutPacketPointerCache;
};

#endif /* mu2e_artdaq_Overlays_ArtFragmentReader_hh */
