#include "artdaq-core-mu2e/Data/CRVGRDataDecoder.hh"

std::unique_ptr<mu2e::CRVGRDataDecoder::CRVGRStatusPacket> mu2e::CRVGRDataDecoder::GetCRVGRStatusPacket(size_t blockIndex) const
{
    //std::cout << "GetCRVGRStatusPacket(" << blockIndex << ")" << std::endl;
	event_.SetupSubEvent();

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;
	std::unique_ptr<CRVGRStatusPacket> output(nullptr);
	output.reset(new CRVGRStatusPacket(*reinterpret_cast<CRVGRStatusPacket const*>(dataPtr->GetData())));
	return output;
}

std::ostream& mu2e::operator<<(std::ostream& os,
			   mu2e::CRVGRDataDecoder::CRVGRStatusPacket const& status ) {	
    os << "CRCErrorCnt: "      << static_cast<uint16_t>(status.CRCErrorCnt) << std::endl
       << "LossCnt: "          << static_cast<uint16_t>(status.LossCnt) << std::endl
       << "PLLlock: "          << static_cast<uint16_t>(status.PLLlock) << std::endl
       << "EventWindowTag0: "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                           << static_cast<uint16_t>(status.EventWindowTag0) << std::endl
	   << "EventWindowTag1: "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                           << static_cast<uint16_t>(status.EventWindowTag1) << std::endl
	   << "BeamOn: "           << static_cast<uint16_t>(status.BeamOn) << std::endl
	   << "lastWindow: "       << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                           << static_cast<uint16_t>(status.lastWindow) << std::endl
	   << "MarkerDelayCnt: "   << static_cast<uint16_t>(status.MarkerDelayCnt) << std::endl
	   << "HeartBeatCnt: "     << static_cast<uint16_t>(status.HeartBeatCnt) << std::endl
	   << "InjectionTs: "      << "0x" << std::hex <<  std::setw(4) << std::setfill('0')
	                           << static_cast<uint16_t>(status.InjectionTs) << std::endl; 
    return os;
  }

std::unique_ptr<mu2e::CRVGRDataDecoder::CRVGRRawPacket> mu2e::CRVGRDataDecoder::GetCRVGRRawPacket(size_t blockIndex) const
{
    //std::cout << "GetCRVGRStatusPacket(" << blockIndex << ")" << std::endl;
	event_.SetupSubEvent();

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;
	std::unique_ptr<CRVGRRawPacket> output(nullptr);
	output.reset(new CRVGRRawPacket(*reinterpret_cast<CRVGRRawPacket const*>(dataPtr->GetData())));
	return output;
}

  std::ostream& mu2e::operator<<(std::ostream& os,
			   mu2e::CRVGRDataDecoder::CRVGRRawPacket const& raw ) {	
    os << "w0 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w0 << std::endl
	   << "w1 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w1 << std::endl
	   << "w2 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w2 << std::endl
	   << "w3 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w3 << std::endl
	   << "w4 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w4 << std::endl
	   << "w5 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w5 << std::endl
	   << "w6 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w6 << std::endl
	   << "w7 "  << "0x" << std::hex <<  std::setw(4) << std::setfill('0') << raw.w7 << std::endl;
    return os;
  }
