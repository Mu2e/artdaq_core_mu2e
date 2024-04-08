#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_SERDESRXDisparityError.h"

DTCLib::DTC_SERDESRXDisparityError::DTC_SERDESRXDisparityError()
	: data_(0) {}

DTCLib::DTC_SERDESRXDisparityError::DTC_SERDESRXDisparityError(std::bitset<2> data)
	: data_(data) {}

DTCLib::DTC_SERDESRXDisparityError::DTC_SERDESRXDisparityError(uint32_t data, DTC_Link_ID link)
{
	std::bitset<32> dataSet = data;
	uint32_t linkBase = static_cast<uint8_t>(link) * 2;
	data_[0] = dataSet[linkBase];
	data_[1] = dataSet[linkBase + 1];
}
