#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_RocDataHeaderPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_RocDataHeaderPacket_h
//------------------------------------------------------------------------------
// bit-packing of the ROC data header packet
//-----------------------------------------------------------------------------
struct RocDataHeaderPacket_t
{
	// clang-format off
                                        // 8 16-byte words in total
                                        // 16-bit word 0
  uint16_t byteCount   : 16;
                                        // 16-bit word 1
  uint16_t unused      : 4;
  uint16_t packetType  : 4;
  uint16_t linkID      : 3;
  uint16_t DtcErrors   : 4;
  uint16_t valid       : 1;
                                        // 16-bit word 2
  uint16_t packetCount : 11;
  uint16_t unused2     : 2;             /*  */
  uint16_t subsystemID : 3;
                                        // 16-bit words 3-5
  uint16_t eventTag[3];
                                        // 16-bit word 6
  uint8_t status       : 8;
  uint8_t version      : 8;
                                        // 16-bit word 7
  uint8_t dtcID        : 8;
  uint8_t onSpill      : 1;
  uint8_t subrun       : 2;
  uint8_t eventMode    : 5;
                                        // error flags - decoding 'status'
	// clang-format on

	int empty() { return (status & 0x01) == 0; }
	int invalid_dr() { return (status & 0x02); }
	int corrupt() { return (status & 0x04); }
	int timeout() { return (status & 0x08); }
	int overflow() { return (status & 0x10); }

	int error_code() { return (status & 0x1e); }

	ulong ewtag()
	{
		ulong x1 = eventTag[0];
		ulong x2 = eventTag[1];
		ulong x3 = eventTag[2];
		ulong ewt = x1 | (x2 << 16) | (x3 << 32);
		return ewt;
	}
};

#endif
