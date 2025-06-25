#include "artdaq-core-mu2e/Overlays/Decoders/MTPDataDecoder.hh"

#include "TRACE/tracemf.h"

#include <vector>

namespace mu2e {
MTPDataDecoder::MTPDataDecoder(DTCLib::DTC_SubEvent const& evt)
	: DTCDataDecoder(evt)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_MTP || hdr->GetVersion() > 1)
		{
			TLOG(TLVL_ERROR) << "MTPDataDecoder CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_MTP) << "/[0,1])";
		}
	}
}

mtp_data_t GetMTPDataPackets(size_t blockIndex) const
{
        mtp_data_t output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

        auto pos = reinterpret_cast<MTPDataPacket const*>(dataPtr->GetData());
        output.reserve(dataPtr->GetHeader()->GetPacketCount());
        size_t packetsProcessed = 0;
        while (packetsProcessed < dataPtr->GetHeader()->GetPacketCount())
        {
                output.emplace_back(pos);
                packetsProcessed += 1;
                pos += 1;
        }

}
