#include "artdaq-core-mu2e/Overlays/DTC_Types/Utilities.h"

#include <cmath>

size_t DTCLib::Utilities::WriteDMABufferSizeWords(std::ostream& output, bool includeDMAWriteSize, size_t data_size, std::streampos& pos, bool restore_pos)
{
	auto pos_save = output.tellp();
	output.seekp(pos);
	size_t bytes_written = 0;
	if (includeDMAWriteSize)
	{
		uint64_t dmaWriteSize = data_size + sizeof(uint64_t) + sizeof(uint64_t);
		output.write(reinterpret_cast<const char*>(&dmaWriteSize), sizeof(uint64_t));
		bytes_written += sizeof(uint64_t);
	}

	uint64_t dmaSize = data_size + sizeof(uint64_t);
	output.write(reinterpret_cast<const char*>(&dmaSize), sizeof(uint64_t));
	bytes_written += sizeof(uint64_t);
	if (restore_pos) {
		output.seekp(pos_save);
	}
	return bytes_written;
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