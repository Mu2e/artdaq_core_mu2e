#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_FIFOFullErrorFlags_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_FIFOFullErrorFlags_h

#include <iomanip>
#include <ostream>

namespace DTCLib {

/// <summary>
/// This structure is used to decode the FIFOFullErrorFlags register values
/// </summary>
struct DTC_FIFOFullErrorFlags
{
	bool OutputData;            ///< Output Data FIFO Full
	bool CFOLinkInput;          ///< CFO Link Input FIFO Full
	bool ReadoutRequestOutput;  ///< Readout Request Output FIFO Full
	bool DataRequestOutput;     ///< Data Request Output FIFO Full
	bool OtherOutput;           ///< Other Output FIFO Full
	bool OutputDCS;             ///< Output DCS FIFO Full
	bool OutputDCSStage2;       ///< Output DCS Stage 2 FIFO Full
	bool DataInput;             ///< Data Input FIFO Full
	bool DCSStatusInput;        ///< DCS Status Input FIFO Full

	/// <summary>
	/// Default Constructor, sets all flags to false
	/// </summary>
	DTC_FIFOFullErrorFlags()
		: OutputData(false), CFOLinkInput(false), ReadoutRequestOutput(false), DataRequestOutput(false), OtherOutput(false), OutputDCS(false), OutputDCSStage2(false), DataInput(false), DCSStatusInput(false) {}

	/// <summary>
	/// Construct a DTC_FIFOFUllErrorFlags instance with the given values
	/// </summary>
	/// <param name="outputData">Output Data FIFO Full</param>
	/// <param name="cfoLinkInput">CFO Link Input FIFO Full</param>
	/// <param name="readoutRequest">Readout Request FIFO Full</param>
	/// <param name="dataRequest">Data Request FIFO Full</param>
	/// <param name="otherOutput"> Other Output FIFO Full</param>
	/// <param name="outputDCS">Output DCS FIFO Full</param>
	/// <param name="outputDCS2">Output DCS Stage 2 FIFO Full</param>
	/// <param name="dataInput">Data Input FIFO Full</param>
	/// <param name="dcsInput">DCS Status Input FIFO Full</param>
	DTC_FIFOFullErrorFlags(bool outputData, bool cfoLinkInput, bool readoutRequest, bool dataRequest, bool otherOutput,
						   bool outputDCS, bool outputDCS2, bool dataInput, bool dcsInput)
		: OutputData(outputData), CFOLinkInput(cfoLinkInput), ReadoutRequestOutput(readoutRequest), DataRequestOutput(dataRequest), OtherOutput(otherOutput), OutputDCS(outputDCS), OutputDCSStage2(outputDCS2), DataInput(dataInput), DCSStatusInput(dcsInput) {}

	/// <summary>
	/// Write the DTC_FIFOFullErrorFlags to stream in JSON format.
	/// Note that the JSON represents an object, calling code should add a key if there's other objects to stream
	/// </summary>
	/// <param name="stream">Stream to write to</param>
	/// <param name="flags">Flags to parse</param>
	/// <returns>Stream object for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_FIFOFullErrorFlags& flags)
	{
		auto formatSet = (stream.flags() & std::ios_base::boolalpha) != 0;
		stream.setf(std::ios_base::boolalpha);
		stream << "{\"OutputData\":" << flags.OutputData << ",\"CFOLinkInput\":" << flags.CFOLinkInput
			   << ",\"ReadoutRequestOutput\":" << flags.ReadoutRequestOutput
			   << ",\"DataRequestOutput\":" << flags.DataRequestOutput << ",\"OtherOutput\":" << flags.OtherOutput
			   << ",\"OutputDCS\":" << flags.OutputDCS << ",\"OutputDCSStage2\":" << flags.OutputDCSStage2
			   << ",\"DataInput\":" << flags.DataInput << ",\"DCSStatusInput\":" << flags.DCSStatusInput << "}";
		if (!formatSet) stream.unsetf(std::ios_base::boolalpha);
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_FIFOFullErrorFlags_h
