#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_EventMode_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_EventMode_h

#include <cstdint>

namespace DTCLib {

struct DTC_EventMode
{
	uint8_t mode0{0};
	uint8_t mode1{0};
	uint8_t mode2{0};
	uint8_t mode3{0};
	uint8_t mode4{0};

	void GetEventMode(const uint8_t* arr, size_t start = 0) const
	{
		const_cast<uint8_t*>(arr)[start] = mode0;
		const_cast<uint8_t*>(arr)[start + 1] = mode1;
		const_cast<uint8_t*>(arr)[start + 2] = mode2;
		const_cast<uint8_t*>(arr)[start + 3] = mode3;
		const_cast<uint8_t*>(arr)[start + 4] = mode4;
	}
	
	bool isOnSpillFlagSet() const 			{return mode4 & 1;}
	bool isSubRunBitSet() const 			{return mode4 & 2;}
	bool isPredictiveSubRunBitSet() const 	{return mode4 & 4;}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_EventMode_h
