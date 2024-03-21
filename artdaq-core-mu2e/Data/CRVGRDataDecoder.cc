#include "artdaq-core-mu2e/Data/CRVGRDataDecoder.hh"

std::unique_ptr<mu2e::CRVGRDataDecoder::CRVGRStatusPacket> mu2e::CRVGRDataDecoder::GetCRVGRStatusPacket(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;

	std::unique_ptr<CRVGRStatusPacket> output(nullptr);
	output.reset(new CRVGRStatusPacket(*reinterpret_cast<CRVGRStatusPacket const*>(dataPtr->GetData())));
	return output;
}

