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
#include <array>

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

		DataBlockHeader()
			: ByteCount(0)
			, Hopcount(0)
			, PacketType(0)
			, ROCID(0)
			, unused1(0)
			, SubsystemID(0)
			, Valid(0)
			, PacketCount(0)
			, unused2(0)
			, TimestampLow(0)
			, TimestampMed(0)
			, TimestampHigh(0)
			, Status(0)
			, FormatVersion(0)
			, DTCID(0)
			, EVBMode(0)
		{}
		uint64_t GetTimestamp() const { return static_cast<uint64_t>(TimestampLow) + (static_cast<uint64_t>(TimestampMed) << 16) + (static_cast<uint64_t>(TimestampHigh) << 32); }
	};

	struct TrackerDataPacket
	{
		uint16_t StrawIndex;

		uint16_t TDC0A;

		uint16_t TDC0B : 8;
		uint32_t TDC0() const { return static_cast<uint32_t>((TDC0B << 16) + TDC0A); }
		uint8_t TOT0 : 4;
		uint8_t EWMCounter : 4;

		uint16_t TDC1A;

		uint16_t TDC1B : 8;
		uint32_t TDC1() const { return static_cast<uint32_t>((TDC1B << 16) + TDC1A); }
		uint8_t TOT1 : 4;
		uint8_t ErrorFlags : 4;

		uint8_t NumADCPackets : 6;
		uint16_t PMP : 10;

		uint16_t ADC00 : 10;
		uint16_t ADC01A : 6;

		uint16_t ADC01B : 4;
		uint16_t ADC01() const { return ADC01A + (ADC01B << 6); }
		uint16_t ADC02 : 10;
		uint8_t unused1 : 2; 
		TrackerDataPacket()
			: StrawIndex(0)
			, TDC0A(0)
			, TDC0B(0)
			, TOT0(0)
			, EWMCounter(0)
			, TDC1A(0)
			, TDC1B(0)
			, TOT1(0)
			, ErrorFlags(0)
			, NumADCPackets(0)
			, PMP(0)
			, ADC00(0)
			, ADC01A(0)
			, ADC01B(0)
			, ADC02(0)
			, unused1(0)
		{}

		void SetTDC0(uint32_t _tdc0)
		{
			TDC0A = _tdc0 & 0xFFFF;
			TDC0B = (_tdc0 >> 16) & 0xFF;
		}

		void SetTDC1(uint32_t _tdc1)
		{
			TDC1A = _tdc1 & 0xFFFF;
			TDC1B = (_tdc1 >> 16) & 0xFF;
		}

		std::vector<adc_t> Waveform() const
		{
			std::vector<adc_t> output;
			output.reserve(3 + NumADCPackets*12);
			output.push_back(ADC00);
			output.push_back(ADC01());
			output.push_back(ADC02);
			for (size_t i=0;i<NumADCPackets;i++){
				TrackerADCPacket const* adcpacket = (reinterpret_cast<TrackerADCPacket const*> (this + 1)) + i;
				output.push_back(adcpacket->ADC0);
				output.push_back(adcpacket->ADC1());
				output.push_back(adcpacket->ADC2);
				output.push_back(adcpacket->ADC3);
				output.push_back(adcpacket->ADC4());
				output.push_back(adcpacket->ADC5);
				output.push_back(adcpacket->ADC6);
				output.push_back(adcpacket->ADC7());
				output.push_back(adcpacket->ADC8);
				output.push_back(adcpacket->ADC9);
				output.push_back(adcpacket->ADC10());
				output.push_back(adcpacket->ADC11);
			}
			return output;
		}

		void SetWaveform(size_t index, adc_t waveform)
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
		uint16_t ADC1() const { return ADC1A + (ADC1B << 6); }
		uint16_t ADC2 : 10;
		uint8_t unused0 : 2; 

		uint16_t ADC3 : 10;
		uint16_t ADC4A : 6;

		uint16_t ADC4B : 4;
		uint16_t ADC4() const { return ADC4A + (ADC4B << 6); }
		uint16_t ADC5 : 10;
		uint8_t unused1 : 2; 

		uint16_t ADC6 : 10;
		uint16_t ADC7A : 6;

		uint16_t ADC7B : 4;
		uint16_t ADC7() const { return ADC7A + (ADC7B << 6); }
		uint16_t ADC8 : 10;
		uint8_t unused2 : 2; 

		uint16_t ADC9 : 10;
		uint16_t ADC10A : 6;

		uint16_t ADC10B : 4;
		uint16_t ADC10() const { return ADC10A + (ADC10B << 6); }
		uint16_t ADC11 : 10;
		uint8_t unused3 : 2; 

		TrackerADCPacket()
			: ADC0(0)
			, ADC1A(0)
			, ADC1B(0)
			, ADC2(0)
			, unused0(0)
			, ADC3(0)
			, ADC4A(0)
			, ADC4B(0)
			, ADC5(0)
			, unused1(0)
			, ADC6(0)
			, ADC7A(0)
			, ADC7B(0)
			, ADC8(0)
			, unused2(0)
			, ADC9(0)
			, ADC10A(0)
			, ADC10B(0)
			, ADC11(0)
			, unused3(0)
		{}


		void SetWaveform(size_t index, adc_t waveform)
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

		CRVROCStatusPacket()
			: unused1(0)
			, PacketType(0)
			, ControllerID(0)
			, ControllerEventWordCount(0)
			, ActiveFEBFlags2(0)
			, unused2(0)
			, ActiveFEBFlags0(0)
			, ActiveFEBFlags1(0)
			, unused3(0)
			, unused4(0)
			, TriggerCount(0)
			, unused5(0)
			, unused6(0)
			, Errors(0)
			, EventType(0)
		{}
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

		CRVHitReadoutPacket()
			: SiPMID(0)
			, HitTime(0)
			, NumSamples(0)
			, WaveformSample0(0)
			, WaveformSample1(0)
			, WaveformSample2(0)
			, WaveformSample3(0)
			, WaveformSample4(0)
			, WaveformSample5(0)
			, WaveformSample6(0)
			, WaveformSample7(0) {}
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

	int GetTrackerData(size_t block_num, std::vector<const TrackerDataPacket *> &output){
		auto hdr = GetHeader(block_num);
		if (hdr == nullptr) return 1;
		if (hdr->SubsystemID != 0)
		{
			TLOG(TLVL_ERROR) << "Trying to get Tracker data packet from non-Tracker DataBlock!";
			return 1;
		}
		uint16_t num_packets = hdr->PacketCount;
		if (blockSizeBytes(block_num) < sizeof(DataBlockHeader) + sizeof(TrackerDataPacket)*num_packets)
		{
			TLOG(TLVL_ERROR) << "Data Block size indicates that Tracker Data is not present! This event is probably corrupt!";
			return 1;
		}

		size_t offset = 0;
		auto pkt = reinterpret_cast<const TrackerDataPacket *>(hdr + 1);
		while (num_packets > offset){
			if (num_packets < offset + pkt->NumADCPackets){
				TLOG(TLVL_ERROR) << "Data Block size indicates that Tracker Data is not present! This event is probably corrupt!";
				return 1;
			}	
			output.push_back(pkt);	
			offset += 1 + pkt->NumADCPackets;
			pkt += 1 + pkt->NumADCPackets;
		}	
		return 0;
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
