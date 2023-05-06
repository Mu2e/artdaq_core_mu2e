#include "artdaq-core-mu2e/Overlays/STMFragment.hh"

std::unique_ptr<mu2e::STMFragment::STMDataPacket> mu2e::STMFragment::GetSTMData(size_t blockIndex) const
{
	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return nullptr;
		
	std::unique_ptr<STMDataPacket> output(nullptr);
	output.reset(new STMDataPacket(*reinterpret_cast<STMDataPacket const*>(dataPtr->GetData())));
	return output;
}
