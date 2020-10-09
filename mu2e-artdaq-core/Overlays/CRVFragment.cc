#include "mu2e-artdaq-core/Overlays/CRVFragment.hh"

std::unique_ptr<mu2e::CRVFragment::CRVROCStatusPacket> mu2e::CRVFragment::GetCRVROCStatusPacket(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;

	std::unique_ptr<CRVROCStatusPacket> output(nullptr);
	output.reset(new CRVROCStatusPacket(*reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData())));
	return output;
}

std::vector<mu2e::CRVFragment::CRVHitReadoutPacket> mu2e::CRVFragment::GetCRVHitReadoutPackets(size_t blockIndex) const
{
	std::vector<CRVHitReadoutPacket> output;
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

	auto crvRocHdr = reinterpret_cast<CRVROCStatusPacket const*>(dataPtr->GetData());
	size_t nHits = (crvRocHdr->ControllerEventWordCount -
					sizeof(CRVROCStatusPacket)) /
				   sizeof(CRVHitReadoutPacket);

	auto pos = reinterpret_cast<CRVHitReadoutPacket const*>(crvRocHdr + 1);
	for (size_t ii = 0; ii < nHits; ++ii) {
		output.emplace_back(*pos);
		pos++;
	}

	return output;
}

