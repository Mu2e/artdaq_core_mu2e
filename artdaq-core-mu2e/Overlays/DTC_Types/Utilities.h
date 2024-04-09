#ifndef artdaq_core_mu2e_Overlays_DTC_Types_Utilities_h
#define artdaq_core_mu2e_Overlays_DTC_Types_Utilities_h

#include <ostream>
#include <string>
#include <utility> // std::pair

namespace DTCLib {

/// <summary>
/// Several useful data manipulation utilities
/// </summary>
struct Utilities
{
	/// <summary>
	/// Create a string with "[value] [unit]" for a given number of bytes, using FormatBytes(bytes)
	/// </summary>
	/// <param name="bytes">Number of bytes to convert to string</param>
	/// <param name="extraUnit">Extra units to be applied after converted byte unit (i.e. "/s")</param>
	/// <returns>String with converted value</returns>
	static std::string FormatByteString(double bytes, std::string extraUnit);
	/// <summary>
	/// Determine the best units for describing a given number of bytes.
	/// Algorithm will divide by 1024, and if the result is greater than 1 and less than 1024, use that unit.
	/// Maximum unit is TB.
	/// </summary>
	/// <param name="bytes">Number of bytes to format</param>
	/// <returns>Pair of Value in "best unit" and string representation of unit (i.e. "KB", "MB", etc)</returns>
	static std::pair<double, std::string> FormatBytes(double bytes);
	/// <summary>
	/// Create a string with "[value] [unit]" for a given number of seconds, using FormatTime(seconds)
	/// </summary>
	/// <param name="seconds">Number of seconds to convert to string</param>
	/// <returns>String with converted value</returns>
	static std::string FormatTimeString(double seconds);
	/// <summary>
	/// Determine the best units for describing a given number of seconds.
	/// Will find largest time span which has unit greater than 1, up to days, down to ns.
	/// </summary>
	/// <param name="seconds">Number of seconds to convert</param>
	/// <returns>Pair of Value in "best unit" and string representation of unit (i.e. "ns", "us", "hours", etc)</returns>
	static std::pair<double, std::string> FormatTime(double seconds);

	/// <summary>
	/// Print out the buffer in hexdump -c format
	/// </summary>
	/// <param name="ptr">Pointer to the buffer</param>
	/// <param name="sz">Size of the buffer</param>
	/// <param name="quietCount">Number of lines to print at the begin/end. Default is 0, which prints entire buffer</param>
	/// <param name="tlvl">TLVL to use for printing (Default 2, or TLVL_INFO)</param>
	static void PrintBuffer(const void* ptr, size_t sz, size_t quietCount = 0, int tlvl = 2 /*TLVL_INFO*/);

	static unsigned getOptionValue(int* index, char** argv[]);
	static unsigned long long getOptionValueLong(int* index, char** argv[]);

	static std::string getOptionString(int* index, char** argv[]);

	static unsigned getLongOptionValue(int* index, char** argv[]);
	static unsigned long long getLongOptionValueLong(int* index, char** argv[]);

	static std::string getLongOptionOption(int* index, char** argv[]);
	static std::string getLongOptionString(int* index, char** argv[]);

	static size_t WriteDMABufferSizeWords(std::ostream& output, bool includeDMAWriteSize, size_t data_size, std::streampos& pos, bool restore_pos);
};
}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_Utilities_h
