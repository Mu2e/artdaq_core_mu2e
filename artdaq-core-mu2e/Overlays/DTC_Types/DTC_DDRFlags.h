#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_DDRFlags_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_DDRFlags_h

#include <ostream>

namespace DTCLib {

/// <summary>
/// This structure is used to decode the DDR Flags register values
/// </summary>
struct DTC_DDRFlags
{
	bool InputFragmentBufferFull;      ///< The input fragment buffer is full
	bool InputFragmentBufferEmpty;     ///< The input fragment buffer is empty
	bool InputFragmentBufferHalfFull;  ///< The input fragment buffer is at least half full
	bool OutputEventBufferFull;        ///< The output event buffer is full
	bool OutputEventBufferEmpty;       ///< The output event buffer is empty
	bool OutputEventBufferHalfFull;    ///< The output event buffer is at least half full

	/// <summary>
	/// Default Constructor, sets all flags to false
	/// </summary>
	DTC_DDRFlags()
		: InputFragmentBufferFull(false)
		, InputFragmentBufferEmpty(false)
		, InputFragmentBufferHalfFull(false)
		, OutputEventBufferFull(false)
		, OutputEventBufferEmpty(false)
		, OutputEventBufferHalfFull(false) {}

	/// <summary>
	/// Construct a DTC_DDRFlags instance with the given values
	/// </summary>
	/// <param name="ifbf">InputFragmentBufferFull value</param>
	/// <param name="ifbe">InputFragmentBufferEmpty</param>
	/// <param name="ifbhf">InputFragmentBufferHalfFull</param>
	/// <param name="ofbf">OutputEventBufferFull</param>
	/// <param name="ofbe">OutputEventBufferEmpty</param>
	/// <param name="ofbhf">OutputEventBufferHalfFull</param>
	DTC_DDRFlags(bool ifbf, bool ifbe, bool ifbhf, bool ofbf, bool ofbe, bool ofbhf)
		: InputFragmentBufferFull(ifbf)
		, InputFragmentBufferEmpty(ifbe)
		, InputFragmentBufferHalfFull(ifbhf)
		, OutputEventBufferFull(ofbf)
		, OutputEventBufferEmpty(ofbe)
		, OutputEventBufferHalfFull(ofbhf) {}

	/// <summary>
	/// Write the DTC_DDRFlags to stream in JSON format.
	/// Note that the JSON represents an object, calling code should add a key if there's other objects to stream
	/// </summary>
	/// <param name="stream">Stream to write to</param>
	/// <param name="flags">Flags to parse</param>
	/// <returns>Stream object for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_DDRFlags& flags)
	{
		auto formatSet = (stream.flags() & std::ios_base::boolalpha) != 0;
		stream.setf(std::ios_base::boolalpha);
		stream << "{\"InputFragmentBufferFull\":" << flags.InputFragmentBufferFull
			   << ",\"InputFragmentBufferEmpty\":" << flags.InputFragmentBufferEmpty
			   << ",\"InputFragmentBufferHalfFull\":" << flags.InputFragmentBufferHalfFull
			   << ",\"OutputEventBufferFull\":" << flags.OutputEventBufferFull
			   << ",\"OutputEventBufferEmpty\":" << flags.OutputEventBufferEmpty
			   << ",\"OutputEventBufferHalfFull\":" << flags.OutputEventBufferHalfFull << "}";
		if (!formatSet) stream.unsetf(std::ios_base::boolalpha);
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_DDRFlags_h
