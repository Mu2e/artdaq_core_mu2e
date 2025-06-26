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
                   : timeStamp0(0), ewt0a(0), ewt0b(0), ewt0c(0),
                     timeStamp1(0), ewt1a(0), ewt1b(0), ewt1c(0)
                 {}


                 uint16_t GetTimestamp(int index) const
                 {
                         if (index == 0)
                           return timeStamp0;
                         else if (index == 1)
                           return timeStamp1;
                         else
                           return 0;
                 }
        };

        typedef std::vector<const MTPDataPacket*> mtp_data_t;

        mtp_data_t GetMTPDataPackets(size_t blockIndex) const;
};
}

#endif  // ARTDAQ_CORE_MU2E_DATA_MTPDATADECODER_HH
