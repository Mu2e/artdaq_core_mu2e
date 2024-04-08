#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_SimMode_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_SimMode_h

#include <ostream>
#include <string>

namespace DTCLib {

/// <summary>
/// The DTC_SimMode enumeration is used to control the behavior of the DTC class.
///
/// DTC_SimMode_Tracker, Calorimeter, CosmicVeto, Performance, and LargeFile activate the mu2esim DTC emulator
/// in the corresponding mode.
/// DTC_SimMode_Disabled does nothing to set up the DTC beyond basic initialization.
/// DTC_SimMode_NoCFO enables the DTC CFO Emulator to send ReadoutRequest and DataRequest packets.
/// DTC_SimMode_ROCEmulator enables the DTC ROC Emulator
/// DTC_SimMode_Loopback enables the SERDES loopback on the DTC
/// </summary>
enum DTC_SimMode
{
	DTC_SimMode_Disabled = 0,
	DTC_SimMode_Tracker = 1,
	DTC_SimMode_Calorimeter = 2,
	DTC_SimMode_CosmicVeto = 3,
	DTC_SimMode_NoCFO = 4,
	DTC_SimMode_ROCEmulator = 5,
	DTC_SimMode_Loopback = 6,
	DTC_SimMode_Performance = 7,
	DTC_SimMode_LargeFile = 8,
	DTC_SimMode_Timeout = 9,
	DTC_SimMode_Event = 10,
	DTC_SimMode_Invalid,
};

/// <summary>
/// The DTC_SimModeConverter converts a DTC_SimMode enumeration value to string or JSON representation
/// </summary>
struct DTC_SimModeConverter
{
	DTC_SimMode mode_;  ///< DTC_SimMode to convert to string

	/// <summary>
	/// Construct a DTC_SimModeConverter instance using the given DTC_SimMode
	/// </summary>
	/// <param name="mode">DTC_SimMode to convert</param>
	explicit DTC_SimModeConverter(DTC_SimMode mode)
		: mode_(mode) {}

	/// <summary>
	/// Parse a string and return the DTC_SimMode which corresponds to it
	///
	/// Will search for SimMode name (see DTC_SimModeConverter::toString(),), or integer value (i.e. 1 =
	/// DTC_SimMode_Tracker, see enumeration definition)
	/// </summary>
	/// <param name="s">String to parse</param>
	/// <returns>DTC_SimMode corresponding to string</returns>
	static DTC_SimMode ConvertToSimMode(std::string s);

	/// <summary>
	/// Convert the DTC_SimMode to its string representation
	/// </summary>
	/// <returns>String representation of DTC_SimMode</returns>
	std::string toString() const
	{
		switch (mode_)
		{
			case DTC_SimMode_Tracker:
				return "Tracker";
			case DTC_SimMode_Calorimeter:
				return "Calorimeter";
			case DTC_SimMode_CosmicVeto:
				return "CosmicVeto";
			case DTC_SimMode_NoCFO:
				return "NoCFO";
			case DTC_SimMode_ROCEmulator:
				return "ROCEmulator";
			case DTC_SimMode_Loopback:
				return "Loopback";
			case DTC_SimMode_Performance:
				return "Performance";
			case DTC_SimMode_LargeFile:
				return "LargeFile";
			case DTC_SimMode_Timeout:
				return "Timeout";
			case DTC_SimMode_Event:
				return "Event";
			case DTC_SimMode_Disabled:
			default:
				return "Disabled";
		}
	}

	/// <summary>
	/// Write a DTC_SimModeConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="mode">DTC_SimModeConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_SimModeConverter& mode)
	{
		stream << "\"DTC_SimMode\":\"" << mode.toString() << "\"";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_SimMode_h
