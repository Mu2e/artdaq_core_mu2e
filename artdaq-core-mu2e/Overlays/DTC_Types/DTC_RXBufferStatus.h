#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_RXBufferStatus_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_RXBufferStatus_h

#include <ostream>
#include <string>

namespace DTCLib {

enum DTC_RXBufferStatus
{
	DTC_RXBufferStatus_Nominal = 0,
	DTC_RXBufferStatus_BufferEmpty = 1,
	DTC_RXBufferStatus_BufferFull = 2,
	DTC_RXBufferStatus_Underflow = 5,
	DTC_RXBufferStatus_Overflow = 6,
	DTC_RXBufferStatus_Unknown = 0x10,
};

/// <summary>
/// The DTC_RXBufferStatusConverter converts a DTC_RXBufferStatus enumeration value to string or JSON representation
/// </summary>
struct DTC_RXBufferStatusConverter
{
	DTC_RXBufferStatus status_;  ///< DTC_RXBufferStatus to convert

	/// <summary>
	/// Construct a DTC_RXBufferStatusConverter instance using the given DTC_RXBufferStatus
	/// </summary>
	/// <param name="status">DTC_RXBufferStatus to convert</param>
	explicit DTC_RXBufferStatusConverter(DTC_RXBufferStatus status)
		: status_(status) {}

	/// <summary>
	/// Convert the DTC_RXBufferStatus to its string representation
	/// </summary>
	/// <returns>String representation of DTC_RXBufferStatus</returns>
	std::string toString() const
	{
		switch (status_)
		{
			case DTC_RXBufferStatus_Nominal:
				return "Nominal";
			case DTC_RXBufferStatus_BufferEmpty:
				return "BufferEmpty";
			case DTC_RXBufferStatus_BufferFull:
				return "BufferFull";
			case DTC_RXBufferStatus_Overflow:
				return "Overflow";
			case DTC_RXBufferStatus_Underflow:
				return "Underflow";
			case DTC_RXBufferStatus_Unknown:
			default:
				return "Unknown";
		}
	}

	/// <summary>
	/// Write a DTC_RXBufferStatusConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="status">DTC_RXBufferStatusConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_RXBufferStatusConverter& status)
	{
		stream << "\"DTC_RXBufferStatus\":\"" << status.toString() << "\"";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_RXBufferStatus_h
