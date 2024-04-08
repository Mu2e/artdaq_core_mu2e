#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_SimMode.h"

DTCLib::DTC_SimMode DTCLib::DTC_SimModeConverter::ConvertToSimMode(std::string modeName)
{
	switch (modeName[0])
	{
		case '1':
			if (modeName.size() > 1) {
				switch (modeName[1])
				{
					case '0':
						return DTC_SimMode_Event;
					default:
						break;
				}
				break;
			}
			return DTC_SimMode_Tracker;
		case 't':
		case 'T':
			return DTC_SimMode_Tracker;
		case '2':
		case 'c':
		case 'C':
			return DTC_SimMode_Calorimeter;
		case '3':
		case 'v':
		case 'V':
			return DTC_SimMode_CosmicVeto;
		case '4':
		case 'n':
		case 'N':
			return DTC_SimMode_NoCFO;
		case '5':
		case 'r':
		case 'R':
			return DTC_SimMode_ROCEmulator;
		case '6':
		case 'l':
		case 'L':
			return DTC_SimMode_Loopback;
		case '7':
		case 'p':
		case 'P':
			return DTC_SimMode_Performance;
		case '8':
		case 'f':
		case 'F':
			return DTC_SimMode_LargeFile;
		case '9':
		case 'o':
		case 'O':
			return DTC_SimMode_Timeout;
		case 'e':
		case 'E':
			return DTC_SimMode_Event;

		case '0':
		case 'D':
		case 'd':
			return DTC_SimMode_Disabled;
		default:
			break;
	}

	try
	{
		auto modeInt = static_cast<DTC_SimMode>(stoi(modeName, nullptr, 10));
		return modeInt != DTC_SimMode_Invalid ? modeInt : DTC_SimMode_Disabled;
	}
	catch (...)
	{
		return DTC_SimMode_Invalid;
	}
}
