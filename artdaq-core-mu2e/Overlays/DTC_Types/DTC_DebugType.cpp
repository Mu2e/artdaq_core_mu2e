#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_DebugType.h"

DTCLib::DTC_DebugType DTCLib::DTC_DebugTypeConverter::ConvertToDebugType(std::string const& type)
{
	switch (type[0])
	{
		case '0':
		case 's':
		case 'S':
			return DTC_DebugType_SpecialSequence;
		case '1':
		case 'e':
		case 'E':
			return DTC_DebugType_ExternalSerial;
		case '2':
		case 'w':
		case 'W':
			return DTC_DebugType_ExternalSerialWithReset;
		case '3':
		case 'r':
		case 'R':
			return DTC_DebugType_RAMTest;
		case '4':
		case 'd':
		case 'D':
			return DTC_DebugType_DDRTest;
		default:
			return DTC_DebugType_Invalid;
	}
}
