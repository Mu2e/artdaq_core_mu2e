#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_SERDESLoopbackMode_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_SERDESLoopbackMode_h

#include <ostream>
#include <string>

namespace DTCLib {

enum DTC_SERDESLoopbackMode
{
	DTC_SERDESLoopbackMode_Disabled = 0,
	DTC_SERDESLoopbackMode_NearPCS = 1,
	DTC_SERDESLoopbackMode_NearPMA = 2,
	DTC_SERDESLoopbackMode_FarPMA = 4,
	DTC_SERDESLoopbackMode_FarPCS = 6,
};

/// <summary>
/// The DTC_SERDESLoopbackModeConverter converts a DTC_SERDESLoopbackMode enumeration value to string or JSON
/// representation
/// </summary>
struct DTC_SERDESLoopbackModeConverter
{
	DTC_SERDESLoopbackMode mode_;  ///< DTC_SERDESLoopbackMode to convert

	/// <summary>
	/// Construct a DTC_SERDESLoopbackModeConverter instance using the given DTC_SERDESLoopbackMode
	/// </summary>
	/// <param name="mode">DTC_SERDESLoopbackMode to convert</param>
	explicit DTC_SERDESLoopbackModeConverter(DTC_SERDESLoopbackMode mode)
		: mode_(mode) {}

	/// <summary>
	/// Convert the DTC_SERDESLoopbackMode to its string representation
	/// </summary>
	/// <returns>String representation of DTC_SERDESLoopbackMode</returns>
	std::string toString() const
	{
		switch (mode_)
		{
			case DTC_SERDESLoopbackMode_Disabled:
				return "Disabled";
			case DTC_SERDESLoopbackMode_NearPCS:
				return "NearPCS";
			case DTC_SERDESLoopbackMode_NearPMA:
				return "NearPMA";
			case DTC_SERDESLoopbackMode_FarPMA:
				return "FarPMA";
			case DTC_SERDESLoopbackMode_FarPCS:
				return "FarPCS";
		}
		return "Unknown";
	}

	/// <summary>
	/// Write a DTC_SERDESLoopbackModeConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="mode">DTC_SERDESLoopbackModeConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_SERDESLoopbackModeConverter& mode)
	{
		stream << "\"DTC_SERDESLoopbackMode\":\"" << mode.toString() << "\"";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_SERDESLoopbackMode_h
