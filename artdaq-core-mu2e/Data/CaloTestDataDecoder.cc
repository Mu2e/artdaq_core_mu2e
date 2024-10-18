#include "artdaq-core-mu2e/Data/CaloTestDataDecoder.hh"

namespace mu2e {

CaloTestDataDecoder::CaloTestDataDecoder(DTCLib::DTC_SubEvent const& evt)
	: DTCDataDecoder(evt)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "CaloTestDataDecoder CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Calorimeter) << "/[0,1])";
		}
	}
}

CaloTestDataDecoder::CaloTestDataDecoder(std::vector<uint8_t> data)
	: DTCDataDecoder(data)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "CaloTestDataDecoder CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Calorimeter) << "/[0,1])";
		}
	}
}

std::vector<std::pair<mu2e::CaloTestDataDecoder::CaloTestHitDataPacket, std::vector<uint16_t>>>* mu2e::CaloTestDataDecoder::GetCalorimeterHitData(size_t blockIndex) const
{
	std::vector<std::pair<mu2e::CaloTestDataDecoder::CaloTestHitDataPacket, std::vector<uint16_t>>> *output = new std::vector<std::pair<mu2e::CaloTestDataDecoder::CaloTestHitDataPacket, std::vector<uint16_t>>>();
  //TODO
	return output;
}

std::vector<std::pair<mu2e::CaloTestDataDecoder::CaloTestHitDataPacket, uint16_t>> mu2e::CaloTestDataDecoder::GetCalorimeterHitsForTrigger(size_t blockIndex) const
{
	std::vector<std::pair<mu2e::CaloTestDataDecoder::CaloTestHitDataPacket,uint16_t>> output;
	//TODO
	return output;
}

} // namespace mu2e
