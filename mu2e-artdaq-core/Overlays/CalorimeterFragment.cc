#include "mu2e-artdaq-core/Overlays/CalorimeterFragment.hh"

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
	auto pos = boardID + (sizeof(CalorimeterBoardID) / 2);

	for (size_t ii = 0; ii < dataPacket->NumberOfHits; ++ii) {
		output.emplace_back(CalorimeterHitReadoutPacket(*reinterpret_cast<CalorimeterHitReadoutPacket const*>(pos)), std::vector<uint16_t>());
		pos += sizeof(CalorimeterHitReadoutPacket) / 2;
		auto nHits = output.back().first.NumberOfSamples;
		output.back().second.resize(nHits);
		memcpy(output.back().second.data(), pos, sizeof(uint16_t) * nHits);
	}
	return output;
}
