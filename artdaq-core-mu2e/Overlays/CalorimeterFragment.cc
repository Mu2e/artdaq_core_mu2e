#include "artdaq-core-mu2e/Overlays/CalorimeterFragment.hh"

std::unique_ptr<mu2e::CalorimeterFragment::CalorimeterDataPacket> mu2e::CalorimeterFragment::GetCalorimeterData(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;
		
	std::unique_ptr<CalorimeterDataPacket> output(nullptr);
	output.reset(new CalorimeterDataPacket(*reinterpret_cast<CalorimeterDataPacket const*>(dataPtr->GetData())));
	return output;
}

std::unique_ptr<mu2e::CalorimeterFragment::CalorimeterBoardID> mu2e::CalorimeterFragment::GetCalorimeterBoardID(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;

	auto dataPacket = reinterpret_cast<CalorimeterDataPacket const*>(dataPtr->GetData());
	auto pos = reinterpret_cast<uint16_t const*>(dataPacket + 1) + dataPacket->NumberOfHits;

	std::unique_ptr<CalorimeterBoardID> output(nullptr);
	output.reset(new CalorimeterBoardID(*reinterpret_cast<CalorimeterBoardID const*>(pos)));
	return output;
}

std::vector<std::pair<mu2e::CalorimeterFragment::CalorimeterHitReadoutPacket, std::vector<uint16_t>>> mu2e::CalorimeterFragment::GetCalorimeterHits(size_t blockIndex) const
{
	std::vector<std::pair<CalorimeterHitReadoutPacket, std::vector<uint16_t>>> output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

	static_assert(sizeof(CalorimeterBoardID) % 2 == 0);
	static_assert(sizeof(CalorimeterHitReadoutPacket) % 2 == 0);

	auto dataPacket = reinterpret_cast<CalorimeterDataPacket const*>(dataPtr->GetData());
	auto boardID = reinterpret_cast<uint16_t const*>(dataPacket + 1) + dataPacket->NumberOfHits;
	// pos is a uint16_t pointer after the BoardID in the data stream
	uint16_t const* pos = boardID + (sizeof(CalorimeterBoardID) / sizeof(uint16_t));

	for (size_t ii = 0; ii < dataPacket->NumberOfHits; ++ii) {
	  // Reinterpret pos as a pointer to a hit readout header
		output.emplace_back(CalorimeterHitReadoutPacket(*reinterpret_cast<CalorimeterHitReadoutPacket const*>(pos)), std::vector<uint16_t>());
		// Step pos past the hit readout
		pos += sizeof(CalorimeterHitReadoutPacket) / sizeof(uint16_t);

		// Setup waveform storage
		auto nSamples = output.back().first.NumberOfSamples;
		output.back().second.resize(nSamples);

		// Copy waveform into output
		memcpy(output.back().second.data(), pos, sizeof(uint16_t) * nSamples);

		// Step pos past waveform
		pos += nSamples;
	}
	return output;
}

std::vector<std::pair<mu2e::CalorimeterFragment::CalorimeterHitReadoutPacket, uint16_t>> mu2e::CalorimeterFragment::GetCalorimeterHitsForTrigger(size_t blockIndex) const
{
	std::vector<std::pair<CalorimeterHitReadoutPacket,uint16_t>> output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

	static_assert(sizeof(CalorimeterBoardID) % 2 == 0);
	static_assert(sizeof(CalorimeterHitReadoutPacket) % 2 == 0);

	auto dataPacket = reinterpret_cast<CalorimeterDataPacket const*>(dataPtr->GetData());
	auto boardID = reinterpret_cast<uint16_t const*>(dataPacket + 1) + dataPacket->NumberOfHits;
	// pos is a uint16_t pointer after the BoardID in the data stream
	uint16_t const* pos = boardID + (sizeof(CalorimeterBoardID) / sizeof(uint16_t));

	for (size_t ii = 0; ii < dataPacket->NumberOfHits; ++ii)
	{
		// Reinterpret pos as a pointer to a hit readout header
		output.emplace_back(CalorimeterHitReadoutPacket(*reinterpret_cast<CalorimeterHitReadoutPacket const*>(pos)), 0);
		// Step pos past the hit readout
		pos += sizeof(CalorimeterHitReadoutPacket) / sizeof(uint16_t);

		output.back().second = *(pos + output.back().first.IndexOfMaxDigitizerSample);

		// Step pos past waveform
		auto nSamples = output.back().first.NumberOfSamples;
		pos += nSamples;
	}
	return output;
}
