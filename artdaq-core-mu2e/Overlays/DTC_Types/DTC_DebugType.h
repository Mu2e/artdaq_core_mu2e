#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_DebugType_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_DebugType_h

#include <ostream>
#include <string>

namespace DTCLib {

enum DTC_DebugType
{
	DTC_DebugType_SpecialSequence = 0,
	DTC_DebugType_ExternalSerial = 1,
	DTC_DebugType_ExternalSerialWithReset = 2,
	DTC_DebugType_RAMTest = 3,
	DTC_DebugType_DDRTest = 4,
	DTC_DebugType_Invalid = 5,
};

/// <summary>
/// The DTC_DebugTypeConverter converts a DTC_DebugType enumeration value to string or JSON representation
/// </summary>
struct DTC_DebugTypeConverter
{
	DTC_DebugType type_;  ///< DTC_DebugType to convert

	/// <summary>
	/// Construct a DTC_DebugTypeConverter instance using the given DTC_DebugType
	/// </summary>
	/// <param name="type">DTC_DebugType to convert</param>
	explicit DTC_DebugTypeConverter(DTC_DebugType type)
		: type_(type) {}

	/// <summary>
	/// Convert the DTC_DebugType to its string representation
	/// </summary>
	/// <returns>String representation of DTC_DebugType</returns>
	std::string toString() const
	{
		switch (type_)
		{
			case DTC_DebugType_SpecialSequence:
				return "Special Sequence";
			case DTC_DebugType_ExternalSerial:
				return "External Serial";
			case DTC_DebugType_ExternalSerialWithReset:
				return "External Serial with FIFO Reset";
			case DTC_DebugType_RAMTest:
				return "FPGA SRAM Error Checking";
			case DTC_DebugType_DDRTest:
				return "DDR3 Memory Error Checking";
			case DTC_DebugType_Invalid:
				return "INVALID!!!";
		}
		return "Unknown";
	}

	static DTC_DebugType ConvertToDebugType(std::string const& type);

	/// <summary>
	/// Write a DTC_DebugTypeConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="type">DTC_DebugTypeConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
#ifndef __CLING__
	friend std::ostream& operator<<(std::ostream& stream, const DTC_DebugTypeConverter& type)
	{
		stream << "\"DTC_DebugType\":\"" << type.toString() << "\"";
		return stream;
	}
#endif
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_DebugType_h
