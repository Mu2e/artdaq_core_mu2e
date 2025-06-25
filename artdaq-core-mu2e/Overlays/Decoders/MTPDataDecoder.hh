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
                uint16_t timeStamp0;
                uint16_t ewt0a;
                uint16_t ewt0b;
                uint16_t ewt0c;
                uint16_t timeStamp1;
                uint16_t ewt1a;
                uint16_t ewt1b;
                uint16_t ewt1c;

		MTPDataPacket()
                  : timeStamp(0), ewt0(0), timeStamp1(0), ewt1(0) {}
	};

        typedef std::vector<const MTPDataPacket*> mtp_data_t;

        mtp_data_t GetMTPDataPackets(size_t blockIndex) const;
};
}

#endif  // ARTDAQ_CORE_MU2E_DATA_MTPDATADECODER_HH
