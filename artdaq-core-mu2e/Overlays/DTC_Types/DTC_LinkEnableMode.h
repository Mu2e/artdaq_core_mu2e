#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_LinkEnableMode_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_LinkEnableMode_h

#include <iomanip>
#include <ostream>

namespace DTCLib {

/// <summary>
/// This structure is used to decode the LinkEnable register value
/// </summary>
struct DTC_LinkEnableMode
{
	bool TransmitEnable;  ///< Whether transmit is enabled on this link
	bool ReceiveEnable;   ///< Whether receive is enabled on this link

	/// <summary>
	/// Default constructor. Sets all enable bits to true.
	/// </summary>
	DTC_LinkEnableMode()
		: TransmitEnable(true), ReceiveEnable(true) {}

	/// <summary>
	/// Construct a DTC_LinkEnableMode instance with the given flags
	/// </summary>
	/// <param name="transmit">Enable TX</param>
	/// <param name="receive">Enable RX</param>
	/// <param name="timing">Enable CFO</param>
	DTC_LinkEnableMode(bool transmit, bool receive)
		: TransmitEnable(transmit), ReceiveEnable(receive) {}

	/// <summary>
	/// Write the DTC_LinkEnableMode to stream in JSON format.
	/// Note that the JSON represents an object, calling code should add a key if there's other objects to stream
	/// </summary>
	/// <param name="stream">Stream to write to</param>
	/// <param name="mode">DTC_LinkEnableMode to parse</param>
	/// <returns>Stream object for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_LinkEnableMode& mode)
	{
		auto formatSet = (stream.flags() & std::ios_base::boolalpha) != 0;
		stream.setf(std::ios_base::boolalpha);
		stream << "{\"TransmitEnable\":" << mode.TransmitEnable << ",\"ReceiveEnable\":" << mode.ReceiveEnable << "}";
		if (!formatSet) stream.unsetf(std::ios_base::boolalpha);
		return stream;
	}

	/// <summary>
	/// Determine if two DTC_LinkEnableMode objects are equal
	/// They are equal if TX, RX and Timing bits are all equal
	/// </summary>
	/// <param name="left">LHS of compare</param>
	/// <param name="right">RHS of compare</param>
	/// <returns>Whether all three bits of both sides are equal</returns>
	friend bool operator==(const DTC_LinkEnableMode& left, const DTC_LinkEnableMode& right)
	{
		return left.TransmitEnable == right.TransmitEnable && left.ReceiveEnable == right.ReceiveEnable;
	}

	/// <summary>
	/// Determine if two DTC_LinkEnableMode objects are not equal
	/// </summary>
	/// <param name="left">LHS of compare</param>
	/// <param name="right">RHS of compare</param>
	/// <returns>!(left == right)</returns>
	friend bool operator!=(const DTC_LinkEnableMode& left, const DTC_LinkEnableMode& right) { return !(left == right); }
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_LinkEnableMode_h
