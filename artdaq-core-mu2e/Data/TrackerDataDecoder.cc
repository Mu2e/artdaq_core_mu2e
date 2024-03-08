#include "artdaq-core-mu2e/Data/TrackerDataDecoder.hh"

#include <vector>

namespace mu2e {
TrackerDataDecoder::TrackerDataDecoder(DTCLib::DTC_SubEvent const& evt)
	: DTCDataDecoder(evt)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Tracker || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "TrackerDataDecoder CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Tracker) << "/[0,1])";
		}
	}
}

TrackerDataDecoder::TrackerDataDecoder(std::vector<uint8_t> data)
	: DTCDataDecoder(data)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Tracker || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "TrackerDataDecoder CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Tracker) << "/[0,1])";
		}
	}
}

TrackerDataDecoder::tracker_data_t TrackerDataDecoder::GetTrackerData(size_t blockIndex, bool readWaveform) const
{
	tracker_data_t output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;
	switch (dataPtr->GetHeader()->GetVersion())
	{
		case 0: {
			auto trackerPacket = reinterpret_cast<TrackerDataPacketV0 const*>(dataPtr->GetData());
			output.emplace_back(Upgrade(trackerPacket), readWaveform ? GetWaveformV0(trackerPacket)
																	 : std::vector<uint16_t>());
		}
		break;
		case 1: {
			auto pos = reinterpret_cast<TrackerDataPacket const*>(dataPtr->GetData());
			auto packetsProcessed = 0;
			output.reserve(dataPtr->GetHeader()->GetPacketCount());

			// Critical Assumption: TrackerDataPacket and TrackerADCPacket are both 16 bytes!
			while (packetsProcessed < dataPtr->GetHeader()->GetPacketCount())
			{
				output.emplace_back(pos, readWaveform ? GetWaveform(pos) : std::vector<uint16_t>());
				auto nPackets = 1 + pos->NumADCPackets;  // TrackerDataPacket + NumADCPackets
				packetsProcessed += nPackets;
				pos += nPackets;
			}
			break;
		}
	}

	return output;
}

std::vector<uint16_t> TrackerDataDecoder::GetWaveformV0(TrackerDataPacketV0 const* trackerPacket) const
{
	std::vector<uint16_t> output(15);
	output[0] = trackerPacket->ADC00;
	output[1] = trackerPacket->ADC01();
	output[2] = trackerPacket->ADC02();
	output[3] = trackerPacket->ADC03;
	output[4] = trackerPacket->ADC04;
	output[5] = trackerPacket->ADC05();
	output[6] = trackerPacket->ADC06();
	output[7] = trackerPacket->ADC07;
	output[8] = trackerPacket->ADC08;
	output[9] = trackerPacket->ADC09();
	output[10] = trackerPacket->ADC10();
	output[11] = trackerPacket->ADC11;
	output[12] = trackerPacket->ADC12;
	output[13] = trackerPacket->ADC13();
	output[14] = trackerPacket->ADC14();

	return output;
}

std::vector<uint16_t> TrackerDataDecoder::GetWaveform(TrackerDataPacket const* trackerHeaderPacket) const
{
	auto adcs = trackerHeaderPacket->NumADCPackets;
	std::vector<uint16_t> output(3 + 12 * adcs);

	output[0] = trackerHeaderPacket->ADC00;
	output[1] = trackerHeaderPacket->ADC01();
	output[2] = trackerHeaderPacket->ADC02;

	auto adcsProcessed = 0;
	auto idx = 2;
	auto trackerADCPacket = reinterpret_cast<TrackerADCPacket const*>(trackerHeaderPacket + 1);
	while (adcsProcessed < adcs)
	{
		output[++idx] = trackerADCPacket->ADC0;
		output[++idx] = trackerADCPacket->ADC1();
		output[++idx] = trackerADCPacket->ADC2;
		output[++idx] = trackerADCPacket->ADC3;
		output[++idx] = trackerADCPacket->ADC4();
		output[++idx] = trackerADCPacket->ADC5;
		output[++idx] = trackerADCPacket->ADC6;
		output[++idx] = trackerADCPacket->ADC7();
		output[++idx] = trackerADCPacket->ADC8;
		output[++idx] = trackerADCPacket->ADC9;
		output[++idx] = trackerADCPacket->ADC10();
		output[++idx] = trackerADCPacket->ADC11;

		adcsProcessed++;
		if (adcsProcessed < adcs)
			trackerADCPacket += 1;  // Go to the next packet, assuming it's a TrackerADCPacket
	}

	return output;
}

const TrackerDataDecoder::TrackerDataPacket* TrackerDataDecoder::Upgrade(const TrackerDataDecoder::TrackerDataPacketV0* input) const
{
	if (input == nullptr) return nullptr;

	upgraded_data_packets_.emplace_back();
	TrackerDataPacket* output = &upgraded_data_packets_.back();
	output->StrawIndex = input->StrawIndex;

	output->TDC0A = input->TDC0;

	output->TDC0B = 0;
	output->TOT0 = input->TOT0 & 0xF;
	output->EWMCounter = 0;

	output->TDC1A = input->TDC1;

	output->TDC1B = 0;
	output->TOT1 = input->TOT1 & 0xF;
	output->ErrorFlags = input->PreprocessingFlags & 0xF;  // Note that we're dropping 4 bits here

	output->NumADCPackets = 1;
	output->PMP = 0;

	return output;
}
}  // namespace mu2e
