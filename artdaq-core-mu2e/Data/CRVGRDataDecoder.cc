#include "artdaq-core-mu2e/Data/CRVGRDataDecoder.hh"

const mu2e::CRVGRDataDecoder::CRVGRStatusPacket& mu2e::CRVGRDataDecoder::GetCRVGRStatusPacket(size_t blockIndex) const
{
	auto block = event_.GetDataBlock(blockIndex);
    void * ptr = ((uint8_t*)block->blockPointer) + (1 * 16); // first package
	return  *reinterpret_cast<const CRVGRStatusPacket*>(ptr);
}

std::ostream& mu2e::operator<<(std::ostream& os,
			   mu2e::CRVGRDataDecoder::CRVGRStatusPacket const& status ) {	
	os << "\"CRVGRStatus\": {" << std::endl
       << "    \"CRCErrorCnt\": "      << std::dec << static_cast<uint16_t>(status.CRCErrorCnt) << "," << std::endl
       << "    \"LossCnt\": "          << std::dec << static_cast<uint16_t>(status.LossCnt) << "," << std::endl
       << "    \"PLLlock\": "          << static_cast<uint16_t>(status.PLLlock) << "," << std::endl
       << "    \"EventWindowTag0\": "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                                   << static_cast<uint16_t>(status.EventWindowTag0) << "," << std::endl
	   << "    \"EventWindowTag1\": "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                                   << static_cast<uint16_t>(status.EventWindowTag1) << "," << std::endl
	   << "    \"BeamOn\": "           << static_cast<uint16_t>(status.BeamOn) << "," << std::endl
	   << "    \"lastWindow\": "       << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                                   << static_cast<uint16_t>(status.lastWindow) << "," << std::endl
	   << "    \"MarkerDelayCnt\": "   << std::dec << static_cast<uint16_t>(status.MarkerDelayCnt) << "," << std::endl
	   << "    \"HeartBeatCnt\": "     << std::dec << static_cast<uint16_t>(status.HeartBeatCnt) << "," << std::endl
	   << "    \"InjectionTs\": "      << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                                   << static_cast<uint16_t>(status.InjectionTs) << std::endl
	   << "}" << std::endl;
    return os;
  }

const mu2e::CRVGRDataDecoder::CRVGRRawPacket& mu2e::CRVGRDataDecoder::GetCRVGRRawPacket(size_t blockIndex) const
{
	auto block = event_.GetDataBlock(blockIndex);
    void * ptr = ((uint8_t*)block->blockPointer) + (1 * 16); // first package
	return  *reinterpret_cast<const CRVGRRawPacket*>(ptr);
}

  std::ostream& mu2e::operator<<(std::ostream& os,
			   mu2e::CRVGRDataDecoder::CRVGRRawPacket const& raw ) {	
	os << "\"CRVGRRaw\" : {\"data\" : ["
       << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w0 << ", " //std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w1 << ", " // std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w2 << ", " // std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w3 << ", " // std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w4 << ", " // std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w5 << ", " // std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w6 << ", " // std::endl
	   << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w7 << "] }" << std::endl;
    return os;
  }
