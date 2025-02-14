#include "artdaq-core-mu2e/Data/CRVDataDecoder.hh"

std::unique_ptr<mu2e::CRVDataDecoder::CRVROCStatusPacket> mu2e::CRVDataDecoder::GetCRVROCStatusPacket(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;

	std::unique_ptr<CRVROCStatusPacket> output(nullptr);
	output.reset(new CRVROCStatusPacket(*reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData())));
	return output;
}

bool mu2e::CRVDataDecoder::GetCRVHits(size_t blockIndex, std::vector<mu2e::CRVDataDecoder::CRVHit> &crvHits) const
{
        crvHits.clear();
        auto dataPtr = dataAtBlockIndex(blockIndex);
        if (dataPtr == nullptr) return false;

        auto crvRocHdr = reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData());
        size_t eventSize = 2*crvRocHdr->ControllerEventWordCount;
        size_t pos = sizeof(CRVROCStatusPacket);

        while(pos<eventSize)
        {
          crvHits.resize(crvHits.size()+1);

          memcpy(&crvHits.back().first, reinterpret_cast<const uint8_t*>(dataPtr->GetData())+pos, sizeof(CRVHitInfo));
          pos += sizeof(CRVHitInfo);

          size_t nWaveformSamples = crvHits.back().first.NumSamples;
          crvHits.back().second.resize(nWaveformSamples);
          memcpy(&crvHits.back().second[0], reinterpret_cast<const uint8_t*>(dataPtr->GetData())+pos, nWaveformSamples*sizeof(CRVHitWaveformSample));
          pos += sizeof(CRVHitWaveformSample)*nWaveformSamples;

          if(pos>eventSize)
          {
            std::cerr << "************************************************" << std::endl;
            std::cerr << "Corrupted data in blockIndex " << blockIndex << std::endl;
            std::cerr << "ROCID " << (uint16_t)crvRocHdr->ControllerID << std::endl;
            std::cerr << "TriggerCount " << crvRocHdr->TriggerCount << std::endl;
            std::cerr << "EventWindowTag " << crvRocHdr->GetEventWindowTag() << std::endl;
            std::cerr << "************************************************" << std::endl;

            crvHits.clear();
            return false;
          }
        }

        return true;
}
