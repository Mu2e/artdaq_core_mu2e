#include "artdaq-core-mu2e/Data/CRVDataDecoder.hh"

const mu2e::CRVDataDecoder::CRVROCStatusPacket& mu2e::CRVDataDecoder::GetCRVROCStatusPacket(size_t blockIndex) const
{
	//auto dataPtr = dataAtBlockIndex(blockIndex);
	//if (dataPtr == nullptr) return nullptr;

	//std::unique_ptr<CRVROCStatusPacket> output(nullptr);
	//output.reset(new CRVROCStatusPacket(*reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData())));
	//return output;
	auto block = event_.GetDataBlock(blockIndex);
  void * ptr = ((uint8_t*)block->blockPointer) + (1 * 16); // first package
	return  *reinterpret_cast<const CRVROCStatusPacket*>(ptr);
}

std::vector<mu2e::CRVDataDecoder::CRVHit> mu2e::CRVDataDecoder::GetCRVHits(size_t blockIndex) const
{
	  //auto dataPtr = dataAtBlockIndex(blockIndex);
	  //if (dataPtr == nullptr) return std::vector<CRVHit>();

	  //auto crvRocHdr = reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData());
    //      size_t eventSize = 2*crvRocHdr->ControllerEventWordCount;
    auto crvRocHdr = GetCRVROCStatusPacket(blockIndex);
    size_t eventSize = 2*crvRocHdr.ControllerEventWordCount;
    size_t pos = sizeof(CRVROCStatusPacket);

    auto block = event_.GetDataBlock(blockIndex);
    void * ptr = ((uint8_t*)block->blockPointer) + (1 * 16);

        std::vector<mu2e::CRVDataDecoder::CRVHit> output;
        while(pos<eventSize)
        {
          output.resize(output.size()+1);

	  memcpy(&output.back().first, reinterpret_cast<const uint8_t*>(ptr)+pos, sizeof(CRVHitInfo));
          pos += sizeof(CRVHitInfo);

          size_t nWaveformSamples = output.back().first.NumSamples;
          output.back().second.resize(nWaveformSamples);
	  memcpy(&output.back().second[0], reinterpret_cast<const uint8_t*>(ptr)+pos, nWaveformSamples*sizeof(CRVHitWaveformSample));
          pos += sizeof(CRVHitWaveformSample)*nWaveformSamples;

          if(pos>eventSize)
          {
            std::cerr << "************************************************" << std::endl;
            std::cerr << "Corrupted data in blockIndex " << blockIndex << std::endl;
            std::cerr << "ROCID " << (uint16_t)crvRocHdr.ControllerID << std::endl;
            std::cerr << "TriggerCount " << crvRocHdr.TriggerCount << std::endl;
            std::cerr << "EventWindowTag " << crvRocHdr.GetEventWindowTag() << std::endl;
            std::cerr << "************************************************" << std::endl;
          }
        }

	return output;
}

std::ostream& mu2e::operator<<(std::ostream& os,
			   mu2e::CRVDataDecoder::CRVROCStatusPacket const& status ) {	
	os << "\"CRVROCStatusPacket\": {"   << std::endl
     << "    \"PacketType\": "        << "0x" << std::hex << static_cast<uint16_t>(status.PacketType) << "," << std::endl
     << "    \"ControllerID\": "      << "0x" << std::hex << static_cast<uint16_t>(status.ControllerID) << "," << std::endl
     << "    \"EventWordCount\": "      << static_cast<uint16_t>(status.ControllerEventWordCount) << "," << std::endl
     << "    \"ActiveFEBFlags\": "    << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
                                      << static_cast<uint32_t>(status.GetActiveFEBFlags().to_ulong()) << "," << std::endl
     << "    \"TriggerCount\": "      << std::dec << static_cast<uint16_t>(status.TriggerCount) << "," << std::endl
     << "    \"MicroBunchStatus\": "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
                                      << static_cast<uint16_t>(status.MicroBunchStatus) << "," << std::endl
     << "    \"EventWindowTag\": "    //<< "0x" << std::hex <<  std::setw(4) << std::setfill('0')
                                      << static_cast<uint32_t>(status.GetEventWindowTag()) << std::endl
	   << "}" << std::endl;
    return os;
  }

  std::ostream& mu2e::operator<<(std::ostream& os,
			   mu2e::CRVDataDecoder::CRVHit const& hit ) {	
	os << "\"CRVHit\": {"   << std::endl
     << "    \"febChannel\": "        << static_cast<uint16_t>(hit.first.febChannel) << "," << std::endl
     << "    \"portNumber\": "        << static_cast<uint16_t>(hit.first.portNumber) << "," << std::endl
     << "    \"controllerNumber\": "  << static_cast<uint16_t>(hit.first.controllerNumber) << "," << std::endl
     << "    \"HitTime\": "           << static_cast<uint16_t>(hit.first.HitTime) << "," << std::endl
     << "    \"NumSamples\": "        << static_cast<uint16_t>(hit.first.NumSamples) << "," << std::endl
     << "    \"ADC\": [";
     for(unsigned int k = 0; k < hit.first.NumSamples; ++k) {
        os << "0x" << std::hex <<  std::setw(3) << std::setfill('0')
           << hit.second[k].ADC << " ";
     }
     os << "    ]" << std::endl 
	      << "}" << std::endl;
    return os;
  }