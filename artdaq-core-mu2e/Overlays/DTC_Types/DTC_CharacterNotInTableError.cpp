#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_CharacterNotInTableError.h"

DTCLib::DTC_CharacterNotInTableError::DTC_CharacterNotInTableError()
	: data_(0) {}

DTCLib::DTC_CharacterNotInTableError::DTC_CharacterNotInTableError(std::bitset<2> data)
	: data_(data) {}

DTCLib::DTC_CharacterNotInTableError::DTC_CharacterNotInTableError(uint32_t data, DTC_Link_ID link)
{
	std::bitset<32> dataSet = data;
	uint32_t linkBase = static_cast<uint8_t>(link) * 2;
	data_[0] = dataSet[linkBase];
	data_[1] = dataSet[linkBase + 1];
}
