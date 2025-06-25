#ifndef ARTDAQ_CORE_MU2E_DATA_MTPDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_MTPDATADECODER_HH

#include "artdaq-core-mu2e/Overlays/Decoders/DTCDataDecoder.hh"
#include <memory>
#include <vector>
#include <bitset>

namespace mu2e {
class MTPDataDecoder : public DTCDataDecoder
{
public:

        explicit MTPDataDecoder(DTCLib::DTC_SubEvent const& evt);

	struct MTPDataPacket
	{
                uint64_t timeStamp0 : 16;
                uint64_t ewt0 : 48;
                uint64_t timeStamp1 : 16;
                uint64_t ewt1 : 48;

		MTPDataPacket()
                  : timeStamp(0), ewt0(0), timeStamp1(0), ewt1(0) {}
	};

        typedef std::vector<const MTPDataPacket*> mtp_data_t;

        mtp_data_t GetMTPDataPackets(size_t blockIndex) const;
};
}

#endif  // ARTDAQ_CORE_MU2E_DATA_MTPDATADECODER_HH
