#include "mu2e-artdaq-core/Overlays/TrackerFragment.hh"

#include <vector>
#include "TrackerFragment.hh"

namespace mu2e {
TrackerFragment::TrackerFragment(artdaq::Fragment const& f)
	: ArtFragment(f)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr.GetSubsystem() != DTCLib::DTC_Subsystem_Tracker || hdr.GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "TrackerFragment CONSTRUCTOR: First block has unsupported type/version " << hdr.GetSubsystem() << "/" << hdr.GetVersion();
		}
	}
}

TrackerFragment::tracker_data_t TrackerFragment::GetTrackerData(size_t blockIndex) const
{
	tracker_data_t output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;
	switch (dataPtr->GetHeader().GetVersion())
	{
		case 0: {
			auto trackerPacket = reinterpret_cast<TrackerDataPacketV0 const*>(dataPtr->GetData());
			output.push_back(std::make_pair(Upgrade(trackerPacket), GetWaveformV0(trackerPacket)));
		}
		break;
		case 1: {
			auto pos = reinterpret_cast<TrackerDataPacket const*>(dataPtr->GetData());
			auto packetsProcessed = 0;

			// Critical Assumption: TrackerDataPacket and TrackerADCPacket are both 16 bytes!
			while (packetsProcessed < dataPtr->GetHeader().GetPacketCount())
			{
				output.push_back(std::make_pair(TrackerDataPacket(*pos), GetWaveform(pos)));
				auto nPackets = 1 + pos->NumADCPackets;  // TrackerDataPacket + NumADCPackets
				packetsProcessed += nPackets;
				pos += nPackets;
			}
			break;
		}
	}

	return output;
}

std::vector<uint16_t> TrackerFragment::GetWaveformV0(TrackerDataPacketV0 const* trackerPacket) const
{
	std::vector<uint16_t> output(14);
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

std::vector<uint16_t> TrackerFragment::GetWaveform(TrackerDataPacket const* trackerHeaderPacket) const
{
	std::vector<uint16_t> output;

	output.push_back(trackerHeaderPacket->ADC00);
	output.push_back(trackerHeaderPacket->ADC01());
	output.push_back(trackerHeaderPacket->ADC02);

	auto adcs = trackerHeaderPacket->NumADCPackets;
	auto adcsProcessed = 0;
	auto trackerADCPacket = reinterpret_cast<TrackerADCPacket const*>(trackerHeaderPacket + 1);
	while (adcsProcessed < adcs)
	{
		output.push_back(trackerADCPacket->ADC0);
		output.push_back(trackerADCPacket->ADC1());
		output.push_back(trackerADCPacket->ADC2);
		output.push_back(trackerADCPacket->ADC3);
		output.push_back(trackerADCPacket->ADC4());
		output.push_back(trackerADCPacket->ADC5);
		output.push_back(trackerADCPacket->ADC6);
		output.push_back(trackerADCPacket->ADC7());
		output.push_back(trackerADCPacket->ADC8);
		output.push_back(trackerADCPacket->ADC9);
		output.push_back(trackerADCPacket->ADC10());
		output.push_back(trackerADCPacket->ADC11);

		adcsProcessed++;
		if (adcsProcessed < adcs)
			trackerADCPacket += 1;  // Go to the next packet, assuming it's a TrackerADCPacket
	}

	return output;
}

TrackerFragment::TrackerDataPacket TrackerFragment::Upgrade(const TrackerFragment::TrackerDataPacketV0* input)
{
	if (input == nullptr) return TrackerDataPacket();
	TrackerDataPacket output;
	output.StrawIndex = input->StrawIndex;

	output.TDC0A = input->TDC0;

	output.TDC0B = 0;
	output.TOT0 = input->TOT0 & 0xF;
	output.EWMCounter = 0;

	output.TDC1A = input->TDC1;

	output.TDC1B = 0;
	output.TOT1 = input->TOT1 & 0xF;
	output.ErrorFlags = input->PreprocessingFlags & 0xF;  // Note that we're dropping 4 bits here

	output.NumADCPackets = 1;
	output.PMP = 0;

	return output;
}
}  // namespace mu2e
