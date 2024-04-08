#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_RXStatus_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_RXStatus_h

#include <ostream>
#include <string>

namespace DTCLib {

enum DTC_RXStatus
{
	DTC_RXStatus_DataOK = 0,
	DTC_RXStatus_SKPAdded = 1,
	DTC_RXStatus_SKPRemoved = 2,
	DTC_RXStatus_ReceiverDetected = 3,
	DTC_RXStatus_DecodeError = 4,
	DTC_RXStatus_ElasticOverflow = 5,
	DTC_RXStatus_ElasticUnderflow = 6,
	DTC_RXStatus_RXDisparityError = 7,
};

/// <summary>
/// The DTC_RXStatusConverter converts a DTC_RXStatus enumeration value to string or JSON representation
/// </summary>
struct DTC_RXStatusConverter
{
	DTC_RXStatus status_;  ///< DTC_RXStatus to convert

	/// <summary>
	/// Construct a DTC_RXStatusConverter instance using the given DTC_RXStatus
	/// </summary>
	/// <param name="status">DTC_RXStatus to convert</param>
	explicit DTC_RXStatusConverter(DTC_RXStatus status);

	/// <summary>
	/// Convert the DTC_RXStatus to its string representation
	/// </summary>
	/// <returns>String representation of DTC_RXStatus</returns>
	std::string toString() const
	{
		switch (status_)
		{
			case DTC_RXStatus_DataOK:
				return "DataOK";
			case DTC_RXStatus_SKPAdded:
				return "SKPAdded";
			case DTC_RXStatus_SKPRemoved:
				return "SKPRemoved";
			case DTC_RXStatus_ReceiverDetected:
				return "ReceiverDetected";
			case DTC_RXStatus_DecodeError:
				return "DecodeErr";
			case DTC_RXStatus_ElasticOverflow:
				return "ElasticOF";
			case DTC_RXStatus_ElasticUnderflow:
				return "ElasticUF";
			case DTC_RXStatus_RXDisparityError:
				return "RXDisparity";
		}
		return "Unknown";
	}

	/// <summary>
	/// Write a DTC_RXStatusConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="status">DTC_RXStatusConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_RXStatusConverter& status)
	{
		stream << "\"DTC_RXStatus\":\"" << status.toString() << "\"";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_RXStatus_h
