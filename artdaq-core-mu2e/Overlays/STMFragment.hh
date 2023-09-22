#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"
#include "STMDAQ-TestBeam/utils/Hex.hh"
#include "STMDAQ-TestBeam/utils/dataVars.hh"
#include "STMDAQ-TestBeam/utils/xml.hh"
#include "STMDAQ-TestBeam/utils/EnvVars.hh"

namespace mu2e {
class STMFragment
{
public:
	explicit STMFragment(artdaq::Fragment const& f)
		: artdaq_fragment_(f) {}

	class STM_tHdr
	{
	public:
		uint16_t sw_tHdr[20];
		STM_tHdr(){};

		// Reverse order - data is little endian.
		uint32_t testWord() const
		{
			uint32_t tmp = (sw_tHdr[1] << 16) | sw_tHdr[0];
			return tmp;
		};
		uint32_t dataSize() const
		{
			uint32_t tmp = (sw_tHdr[3] << 16) | sw_tHdr[2];
			return tmp;
		};
		uint32_t sliceNumber() const
		{
			uint32_t tmp = ((sw_tHdr[5] << 16) | sw_tHdr[4]);
			return tmp;
		};
		uint32_t triggerNumber() const
		{
			uint32_t tmp = ((sw_tHdr[7] << 16) | sw_tHdr[6]);
			return tmp;
		};
		uint16_t mode() const { return (sw_tHdr[8] & 0b1111000000000000) >> 12; };
		uint16_t channel() const { return (sw_tHdr[8] & 0b0000111100000000) >> 8; };
		uint16_t type() const { return (sw_tHdr[8] & 0b0000000011111111); };
		uint64_t triggerTime() const
		{
			uint64_t T1 = sw_tHdr[12], T2 = sw_tHdr[11], T3 = sw_tHdr[10], T4 = sw_tHdr[9], time;
			T1 = T1 << 48;
			T2 = T2 << 32;
			T3 = T3 << 16;
			time = T1 | T2 | T3 | T4;
			return time;
		};
		uint32_t adcOffset() const
		{
			uint32_t tmp = (sw_tHdr[14] << 16) | sw_tHdr[13];
			return tmp;
		};
		uint16_t droppedPackets() const { return sw_tHdr[15]; }
		uint64_t unixTime() const
		{
			uint64_t T1 = sw_tHdr[19], T2 = sw_tHdr[18], T3 = sw_tHdr[17], T4 = sw_tHdr[16], time;
			T1 = T1 << 48;
			T2 = T2 << 32;
			T3 = T3 << 16;
			time = T1 | T2 | T3 | T4;
			return time;
		};  // Epoch time in milliseconds.
	};

	class STM_sHdr
	{
	public:
		uint16_t sw_sHdr[8];
		STM_sHdr(){};

		// Reverse order - data is little endian.
		uint32_t sliceNumber() const
		{
			uint32_t tmp = (sw_sHdr[1] << 16) | sw_sHdr[0];
			return tmp;
		};
		uint32_t sliceSize() const
		{
			uint32_t tmp = (sw_sHdr[3] << 16) | sw_sHdr[2];
			return tmp;
		};
		uint64_t adcTime() const
		{
			uint64_t T1 = sw_sHdr[7], T2 = sw_sHdr[6], T3 = sw_sHdr[5], T4 = sw_sHdr[4], time;
			T1 = T1 << 48;
			T2 = T2 << 32;
			T3 = T3 << 16;
			time = T1 | T2 | T3 | T4;
 			return time;
		};
	};

	STM_tHdr const* GetTHdr()
	{
		return reinterpret_cast<STM_tHdr const*>(artdaq_fragment_.dataBegin());
	}

	STM_sHdr const* GetSHdr() {
		return reinterpret_cast<STM_sHdr const*>(GetTHdr() + 1);
	}

	uint16_t const* DataBegin() {
		return reinterpret_cast<uint16_t const*>(GetSHdr() + 1);
	}

	uint16_t const* DataEnd() {
		return DataBegin() + GetSHdr()->sliceSize();
	}


  class STM_fw_pHdr{
  public:
    uint16_t _fw_pHdr[fw_pHdr_Len];
    STM_fw_pHdr(){};
    uint32_t pNum() const
    {
      uint32_t pNum1 = _fw_pHdr[fw_pHdr_pNum1], pNum2 = _fw_pHdr[fw_pHdr_pNum2], pNum;
      pNum2 = pNum2 >> 16;
      pNum = pNum2 | pNum1;
      return pNum;
    };
    int16_t test() const
    {
      int16_t test = _fw_pHdr[fw_pHdr_end];
      return test;
    };
  };

        class STM_fw_tHdr{
	public:
	  uint16_t _fw_tHdr[fw_tHdr_Len];
	  fw_tHdr fw_tHdr_index;
	  // TODO: get the indexes of each element from the struct defined in dataVars.hh
	  STM_fw_tHdr(){};

	  // Defining members of the header;
	  uint64_t DTCclk() const
	  {
	    uint64_t DTCclk0 = _fw_tHdr[fw_tHdr_index.Ch_DTCclk_0], DTCclk1 = _fw_tHdr[fw_tHdr_index.DTCclk_1], DTCclk2 = _fw_tHdr[fw_tHdr_index.DTCclk_2] , DTCclk3 = _fw_tHdr[fw_tHdr_index.DTCclk_3];
	    DTCclk0 = DTCclk0 & 0xFF00;
	    DTCclk0 = DTCclk0 >> 8;
	    DTCclk3 = DTCclk3 << 40;
	    DTCclk2 = DTCclk2 << 24;
	    DTCclk1 = DTCclk1 << 8;
	    DTCclk0 = DTCclk3 | DTCclk2 | DTCclk1 | DTCclk0;
	    return DTCclk0;
	  };
	  
	  uint64_t EvNum() const
	  {
	    uint64_t EvNum0 = _fw_tHdr[fw_tHdr_index.EvNum_0], EvNum1 = _fw_tHdr[fw_tHdr_index.EvNum_1], EvNum2 = _fw_tHdr[fw_tHdr_index.EvNum_2];
	    EvNum2 = EvNum2 << 32;
	    EvNum1 = EvNum1 << 16;
	    EvNum0 = EvNum2 | EvNum1 | EvNum0;
	    return EvNum0;
	  };

	  uint64_t ADCclk() const
	  {
	    uint64_t ADCclk0 = _fw_tHdr[fw_tHdr_index.ADCclk_0], ADCclk1 = _fw_tHdr[fw_tHdr_index.ADCclk_1], ADCclk2 = _fw_tHdr[fw_tHdr_index.ADCclk_2], ADCclk3 = _fw_tHdr[fw_tHdr_index.ADCclk_3];
	    ADCclk3 = ADCclk3 << 48;
	    ADCclk2 = ADCclk2 << 32;
	    ADCclk1 = ADCclk1 << 16;
	    ADCclk0 = ADCclk3 | ADCclk2 | ADCclk1 | ADCclk0;
	    return ADCclk0;
	  };

	  uint64_t EWT() const 
	  {
	    uint64_t EWT0 = _fw_tHdr[fw_tHdr_index.EWT_0], EWT1 = _fw_tHdr[fw_tHdr_index.EWT_1], EWT2 = _fw_tHdr[fw_tHdr_index.EWT_2];
	    EWT2 = EWT2 << 32;
	    EWT1 = EWT1 << 16;
	    EWT0 = EWT2 | EWT1 | EWT0;
	    return EWT0;
	  };

	  uint64_t EM() const 
	  {
	    uint64_t EM0 = _fw_tHdr[fw_tHdr_index.EM_0], EM1 = _fw_tHdr[fw_tHdr_index.EM_1], EM2 = _fw_tHdr[fw_tHdr_index.EM_2_DRTDC];
	    EM2 = EM2 & 0xFF00;
	    EM2 = EM2 << 24;
	    EM1 = EM1 << 16;
	    EM0 = EM2 | EM1 | EM0;
	    return EM0;
	  };

	  uint16_t DRTDC() const 
	  {
	    uint16_t _DRTDC = _fw_tHdr[fw_tHdr_index.EM_2_DRTDC];
	    _DRTDC = _DRTDC & 0xFF;
	    return _DRTDC;
	  };

	  uint16_t EvStart() const
	  {
	    uint16_t _EvStart = _fw_tHdr[fw_tHdr_index.EvStart];
	    return _EvStart;
	  };

	  uint16_t EvLen() const
	  {
	    uint16_t _EvLen = _fw_tHdr[fw_tHdr_index.EvLen];
	    return _EvLen;
	  };
	  uint16_t channel() const 
	  {
	    uint16_t _channel = _fw_tHdr[fw_tHdr_index.Ch_DTCclk_0];
	    _channel = _channel & 0x00FF;
	    return _channel;
	  };
        };

private:
	artdaq::Fragment const& artdaq_fragment_;
};

/*
std::ostream& operator<<(std::ostream& os, const STMFragment::STMDataPacket& data) {
  os << std::setw(4) << std::setfill('0') << std::right << std::hex << data.FixedHeaderWord[0] << ", " << data.FixedHeaderWord[1] << ", " << data.FixedHeaderWord[2] << ", " << data.FixedHeaderWord[3] << ", " << data.FixedHeaderWord[4] << ", " << data.FixedHeaderWord[5] << ", " << data.FixedHeaderWord[6] << ", " << data.FixedHeaderWord[7] << std::endl;
  return os;
}
*/

static constexpr int sw_tHdr_size_bytes = sizeof(STMFragment::STM_tHdr);
static constexpr int sw_tHdr_size_words = sw_tHdr_size_bytes / 2;
static constexpr int sw_sHdr_size_bytes = sizeof(STMFragment::STM_sHdr);
static constexpr int sw_sHdr_size_words = sw_sHdr_size_bytes / 2;

}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
