#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_PRBSMode_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_PRBSMode_h

#include <ostream>
#include <string>

namespace DTCLib {

enum DTC_PRBSMode
{
	DTC_PRBSMode_Normal = 0,
	DTC_PRBSMode_7 = 1,
	DTC_PRBSMode_15 = 2,
	DTC_PRBSMode_23 = 3,
	DTC_PRBSMode_31 = 4,
	DTC_PRBSMode_PCIExpress = 5,
	DTC_PRBSMode_2UISquare = 6,
	DTC_PRBSMode_20UISquare = 7,
};

struct DTC_PRBSModeConverter
{
	DTC_PRBSMode mode_;  ///< DTC_PRBSMode to convert

	/// <summary>
	/// Construct a DTC_RXStatusConverter instance using the given DTC_RXStatus
	/// </summary>
	/// <param name="status">DTC_RXStatus to convert</param>
	explicit DTC_PRBSModeConverter(DTC_PRBSMode status)
		: mode_(status) {}

	/// <summary>
	/// Convert the DTC_RXStatus to its string representation
	/// </summary>
	/// <returns>String representation of DTC_RXStatus</returns>
	std::string toString() const
	{
		switch (mode_)
		{
			case DTC_PRBSMode_Normal:
				return "Normal";
			case DTC_PRBSMode_7:
				return "PRBS-7";
			case DTC_PRBSMode_15:
				return "PRBS-15";
			case DTC_PRBSMode_23:
				return "PRBS-23";
			case DTC_PRBSMode_31:
				return "PRBS-31";
			case DTC_PRBSMode_PCIExpress:
				return "PCIExpress";
			case DTC_PRBSMode_2UISquare:
				return "2UISquare";
			case DTC_PRBSMode_20UISquare:
				return "20UISquare";
		}
		return "Unknown";
	}

	friend std::ostream& operator<<(std::ostream& stream, const DTC_PRBSModeConverter& mode)
	{
		stream << "\"DTC_PRBSMode\":\"" << mode.toString() << "\"";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_PRBSMode_h
