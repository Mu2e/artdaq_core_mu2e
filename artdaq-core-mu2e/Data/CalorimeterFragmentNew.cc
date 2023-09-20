#include "artdaq-core-mu2e/Data/CalorimeterFragmentNew.hh"

CalorimeterFragmentNew::CalorimeterFragmentNew(DTCLib::DTC_SubEvent const& evt)
	: ArtFragment(evt)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);// Return pointer to beginning of DataBlock at given DataBlock index( returns type DTCLib::DTC_DataBlock)
		auto hdr = dataPtr->GetHeader(); // get the header
		// check that the subsystem is the calo and the version is correct:
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "CalorimeterFragmentNew CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Calorimeter) << "/[0,1])";
		}
	}
}

CalorimeterFragmentNew::CalorimeterFragmentNew(std::vector<uint8_t> data)
	: ArtFragment(data)
{
	if (block_count() > 0) //event_.GetDataBlockCount() > 0
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "CalorimeterFragmentNew CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Calorimeter) << "/[0,1])";
		}
	}
}


// Get Calo Hit Data Packet
std::vector<std::pair<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket, std::vector<uint16_t>>> mu2e::CalorimeterFragmentNew::GetCalorimeterHitData(size_t blockIndex) const
{
  // a pair is created mapping the data packet to set of hits (?)
	std::vector<std::pair<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket, std::vector<uint16_t>>> output;
  
  // get data block at given index
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

	// check size of hit data packet
	static_assert(sizeof(mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket) % 2 == 0);
	
	auto dataPacket = reinterpret_cast<CalorimeterHitDataPacket const*>(dataPtr->GetData());
	
	// pos is a uint16_t pointer after the BoardID in the data stream
	uint16_t const* pos = 0;//assumes this is the first piece of information

	// loop over samples:
	unsigned int count = 0;
	while(count < dataPacket->NumberOfSamples){
	  
	  // Reinterpret pos as a pointer to a hit readout header
	  	output.emplace_back(mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket(*reinterpret_cast<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket const*>(pos)), std::vector<uint16_t>()); //Construct and insert element at the end, Converts between types by reinterpreting the underlying bit pattern. 
		
		// Step pos past the hit readout
		pos += sizeof(mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket) / sizeof(uint16_t);

		// Setup waveform storage
		// find number of samples from output
		auto nSamples = output.back().first.NumberOfSamples;
		// resize the vector part to nSamples
		output.back().second.resize(nSamples);

		// Copy waveform into output
		memcpy(output.back().second.data(), pos, sizeof(uint16_t) * nSamples);

		// Step pos past waveform
		pos += nSamples;
		count++;
	}
	return output;
}

std::vector<std::pair<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket, uint16_t>> mu2e::CalorimeterFragmentNew::GetCalorimeterHitsForTrigger(size_t blockIndex) const
{
	std::vector<std::pair<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket,uint16_t>> output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

	static_assert(sizeof(CalorimeterHitDataPacket) % 2 == 0);

	auto dataPacket = reinterpret_cast<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket const*>(dataPtr->GetData());
	
	uint16_t const* pos = 0;

	// loop over samples:
	unsigned int count = 0;
	while(count < dataPacket->NumberOfSamples){
		// Reinterpret pos as a pointer to a hit readout header
		output.emplace_back(mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket(*reinterpret_cast<CalorimeterHitDataPacket const*>(pos)), 0);
		// Step pos past the hit readout
		pos += sizeof(mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket) / sizeof(uint16_t);

		output.back().second = *(pos + output.back().first.IndexOfMaxDigitizerSample);

		// Step pos past waveform
		auto nSamples = output.back().first.NumberOfSamples;
		pos += nSamples;
	}
	return output;
}
