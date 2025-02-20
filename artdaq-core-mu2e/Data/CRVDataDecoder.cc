#include "artdaq-core-mu2e/Data/CRVDataDecoder.hh"

//for CrvDigis and global run
std::unique_ptr<mu2e::CRVDataDecoder::CRVROCStatusPacket> mu2e::CRVDataDecoder::GetCRVROCStatusPacket(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;

	std::unique_ptr<CRVROCStatusPacket> output(nullptr);
	output.reset(new CRVROCStatusPacket(*reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData())));
	return output;
}

//for CrvDigis
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

//for global run
bool mu2e::CRVDataDecoder::GetCRVGlobalRunInfo(size_t blockIndex, mu2e::CRVDataDecoder::CRVGlobalRunInfo &globalRunInfo) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return false;

        auto crvRocHdr = reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData());
        size_t eventSize = 2*crvRocHdr->ControllerEventWordCount;
        if(sizeof(CRVROCStatusPacket)+sizeof(CRVGlobalRunInfo)>eventSize) return false;

        memcpy(&globalRunInfo, reinterpret_cast<const uint8_t*>(dataPtr->GetData())+sizeof(CRVROCStatusPacket), sizeof(CRVGlobalRunInfo));
        return true;
}

bool mu2e::CRVDataDecoder::GetCRVGlobalRunPayload(size_t blockIndex, std::vector<uint16_t> &globalRunPayload) const
{
        globalRunPayload.clear();
        auto dataPtr = dataAtBlockIndex(blockIndex);
        if (dataPtr == nullptr) return false;

        auto crvRocHdr = reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData());
        size_t eventSize = 2*crvRocHdr->ControllerEventWordCount;
        size_t pos = sizeof(CRVROCStatusPacket)+sizeof(CRVGlobalRunInfo);
        if(pos>eventSize) return false;

        size_t payloadSize = (eventSize-pos)/2;
        globalRunPayload.resize(payloadSize);
        for(size_t i=0; i<payloadSize; ++i)
        {
          memcpy(&globalRunPayload.at(i), reinterpret_cast<const uint8_t*>(dataPtr->GetData())+pos, sizeof(uint16_t));
          pos+=sizeof(uint16_t);
        }

        return true;
}
