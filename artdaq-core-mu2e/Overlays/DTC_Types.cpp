#include "DTC_Types.h"

#include <iomanip>
#include <sstream>
#include <cmath>

#include "TRACE/tracemf.h"

DTCLib::DTC_RXStatusConverter::DTC_RXStatusConverter(DTC_RXStatus status)
	: status_(status) {}

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

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag()
	: event_tag_(0) {}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const uint64_t event_tag)
	: event_tag_(event_tag) {}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const uint32_t event_tag_low, const uint16_t event_tag_high)
{
	SetEventWindowTag(event_tag_low, event_tag_high);
}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const uint8_t* timeArr, int offset)
{
	auto arr = reinterpret_cast<const uint64_t*>(timeArr + offset);
	event_tag_ = *arr;
}

DTCLib::DTC_EventWindowTag::DTC_EventWindowTag(const std::bitset<48> event_tag)
	: event_tag_(event_tag.to_ullong()) {}

void DTCLib::DTC_EventWindowTag::SetEventWindowTag(const uint32_t event_tag_low, const uint16_t event_tag_high)
{
	uint64_t event_tag_temp = event_tag_high;
	event_tag_temp = event_tag_temp << 32;
	event_tag_ = event_tag_low + event_tag_temp;
}

void DTCLib::DTC_EventWindowTag::GetEventWindowTag(const uint8_t* timeArr, int offset) const
{
	for (auto i = 0; i < 6; i++)
	{
		const_cast<uint8_t*>(timeArr)[i + offset] = static_cast<uint8_t>(event_tag_ >> i * 8);
	}
}

std::string DTCLib::DTC_EventWindowTag::toJSON(bool arrayMode) const
{
	std::stringstream ss;
	if (arrayMode)
	{
		uint8_t ts[6];
		GetEventWindowTag(ts, 0);
		ss << "\t\"timestamp\": [\n" << static_cast<int>(ts[0]) << ",\n";
		ss << static_cast<int>(ts[1]) << ",\n";
		ss << static_cast<int>(ts[2]) << ",\n";
		ss << static_cast<int>(ts[3]) << ",\n";
		ss << static_cast<int>(ts[4]) << ",\n";
		ss << static_cast<int>(ts[5]) << "\n]";
	}
	else
	{
		ss << "\t\"timestamp\": " << event_tag_;
	}
	return ss.str();
}

std::string DTCLib::DTC_EventWindowTag::toPacketFormat() const
{
	uint8_t ts[6]{0,0,0,0,0,0};
	GetEventWindowTag(ts, 0);
	std::stringstream ss;
	ss << std::setfill('0') << std::hex;
	ss << "0x" << std::setw(6) << static_cast<int>(ts[1]) << "\t"
	   << "0x" << std::setw(6) << static_cast<int>(ts[0]) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(ts[3]) << "\t"
	   << "0x" << std::setw(6) << static_cast<int>(ts[2]) << "\n";
	ss << "0x" << std::setw(6) << static_cast<int>(ts[5]) << "\t"
	   << "0x" << std::setw(6) << static_cast<int>(ts[4]) << "\n";
	return ss.str();
}

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

std::string DTCLib::Utilities::FormatByteString(double bytes, std::string extraUnit)
{
	auto res = FormatBytes(bytes);
	std::stringstream s;
	s << std::setprecision(5) << res.first << " " << res.second << extraUnit << " ("
	  << std::to_string(static_cast<uint64_t>(bytes)) << " bytes" << extraUnit << ")";
	return s.str();
}

std::pair<double, std::string> DTCLib::Utilities::FormatBytes(double bytes)
{
	auto val = bytes;
	auto unit = "bytes";
	auto kb = bytes / 1024.0;

	if (kb > 1)
	{
		auto mb = kb / 1024.0;
		if (mb > 1)
		{
			auto gb = mb / 1024.0;
			if (gb > 1)
			{
				auto tb = gb / 1024.0;
				if (tb > 1)
				{
					val = tb;
					unit = "TB";
				}
				else
				{
					val = gb;
					unit = "GB";
				}
			}
			else
			{
				val = mb;
				unit = "MB";
			}
		}
		else
		{
			val = kb;
			unit = "KB";
		}
	}

	return std::make_pair(val, unit);
}

std::string DTCLib::Utilities::FormatTimeString(double seconds)
{
	auto res = FormatTime(seconds);
	std::stringstream s;
	s << std::setprecision(5) << res.first << " " << res.second;
	return s.str();
}

std::pair<double, std::string> DTCLib::Utilities::FormatTime(double seconds)
{
	auto val = seconds;
	auto unit = "s";

	if (seconds > 1)
	{
		auto min = seconds / 60.0;
		if (min > 1)
		{
			auto ho = min / 60.0;
			if (ho > 1)
			{
				auto day = ho / 24.0;
				if (day > 1)
				{
					val = day;
					unit = "days";
				}
				else
				{
					val = ho;
					unit = "hours";
				}
			}
			else
			{
				val = min;
				unit = "minutes";
			}
		}
	}
	else
	{
		auto ms = seconds * 1000;
		if (ms > 1)
		{
			val = ms;
			unit = "ms";
		}
		else
		{
			auto us = ms * 1000;
			if (us > 1)
			{
				val = us;
				unit = "us";
			}
			else
			{
				auto ns = us * 1000;
				val = ns;
				unit = "ns";
			}
		}
	}

	return std::make_pair(val, unit);
}

void DTCLib::Utilities::PrintBuffer(const void* ptr, size_t sz, size_t quietCount, int tlvl)
{
	auto maxLine = static_cast<unsigned>(ceil((sz) / 16.0));
	for (unsigned line = 0; line < maxLine; ++line)
	{
		std::stringstream ostr;
		ostr << "0x" << std::hex << std::setw(5) << std::setfill('0') << line << "0: ";
		for (unsigned byte = 0; byte < 8; ++byte)
		{
			if (line * 16 + 2 * byte < sz)
			{
				auto thisWord = reinterpret_cast<const uint16_t*>(ptr)[line * 8 + byte];
				ostr << std::setw(4) << static_cast<int>(thisWord) << " ";
			}
		}
		TLOG(tlvl) << ostr.str();
		if (quietCount > 0 && maxLine > quietCount * 2 && line == (quietCount - 1))
		{
			line = static_cast<unsigned>(ceil((sz) / 16.0)) - (1 + quietCount);
		}
	}
}

unsigned DTCLib::Utilities::getOptionValue(int* index, char** argv[])
{
	auto arg = (*argv)[*index];
	if (arg[2] == '\0')
	{
		(*index)++;
		unsigned ret = strtoul((*argv)[*index], nullptr, 0);
		if (ret == 0 && (*argv)[*index][0] != '0')  // No option given
		{
			(*index)--;
		}
		return ret;
	}
	auto offset = 2;
	if (arg[2] == '=')
	{
		offset = 3;
	}

	return strtoul(&arg[offset], nullptr, 0);
}
unsigned long long DTCLib::Utilities::getOptionValueLong(int* index, char** argv[])
{
	auto arg = (*argv)[*index];
	if (arg[2] == '\0')
	{
		(*index)++;
		unsigned long long ret = strtoull((*argv)[*index], nullptr, 0);
		if (ret == 0 && (*argv)[*index][0] != '0')  // No option given
		{
			(*index)--;
		}
		return ret;
	}
	auto offset = 2;
	if (arg[2] == '=')
	{
		offset = 3;
	}

	return strtoull(&arg[offset], nullptr, 0);
}

std::string DTCLib::Utilities::getOptionString(int* index, char** argv[])
{
	auto arg = (*argv)[*index];
	if (arg[2] == '\0')
	{
		(*index)++;
		return std::string((*argv)[*index]);
	}
	auto offset = 2;
	if (arg[2] == '=')
	{
		offset = 3;
	}

	return std::string(&arg[offset]);
}

unsigned DTCLib::Utilities::getLongOptionValue(int* index, char** argv[])
{
	auto arg = std::string((*argv)[*index]);
	auto pos = arg.find('=');

	if (pos == std::string::npos)
	{
		(*index)++;
		unsigned ret = strtoul((*argv)[*index], nullptr, 0);
		if (ret == 0 && (*argv)[*index][0] != '0')  // No option given
		{
			(*index)--;
		}
		return ret;
	}

	return strtoul(&arg[++pos], nullptr, 0);
}
unsigned long long DTCLib::Utilities::getLongOptionValueLong(int* index, char** argv[])
{
	auto arg = std::string((*argv)[*index]);
	auto pos = arg.find('=');

	if (pos == std::string::npos)
	{
		(*index)++;
		unsigned long long ret = strtoull((*argv)[*index], nullptr, 0);
		if (ret == 0 && (*argv)[*index][0] != '0')  // No option given
		{
			(*index)--;
		}
		return ret;
	}

	return strtoull(&arg[++pos], nullptr, 0);
}

std::string DTCLib::Utilities::getLongOptionOption(int* index, char** argv[])
{
	auto arg = std::string((*argv)[*index]);
	auto pos = arg.find('=');

	if (pos == std::string::npos)
	{
		return arg;
	}
	else
	{
		return arg.substr(0, pos - 1);
	}
}

std::string DTCLib::Utilities::getLongOptionString(int* index, char** argv[])
{
	auto arg = std::string((*argv)[*index]);

	if (arg.find('=') == std::string::npos)
	{
		return std::string((*argv)[++(*index)]);
	}
	else
	{
		return arg.substr(arg.find('='));
	}
}