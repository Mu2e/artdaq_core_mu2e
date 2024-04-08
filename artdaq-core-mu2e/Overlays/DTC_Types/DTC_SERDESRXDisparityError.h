#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_SERDESRXDisparityError_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_SERDESRXDisparityError_h

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"

#include <bitset>
#include <cstdint>
#include <ostream>

namespace DTCLib {

/// <summary>
/// This class is used to decode the SERDES RX Disparity Error register
/// </summary>
class DTC_SERDESRXDisparityError
{
	std::bitset<2> data_;

public:
	/// <summary>
	/// Default Constructor
	/// </summary>
	DTC_SERDESRXDisparityError();
	/// <summary>
	/// Construct a DTC_SERDESRXDisparityError using the given bitset
	/// </summary>
	/// <param name="data">Bits to use for initialization</param>
	explicit DTC_SERDESRXDisparityError(std::bitset<2> data);
	/// <summary>
	/// Construct a DTC_SERDESRXDisparityError using the register value and a Link ID
	/// </summary>
	/// <param name="data">Register value to read error bits from</param>
	/// <param name="link">Link to read</param>
	DTC_SERDESRXDisparityError(uint32_t data, DTC_Link_ID link);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="in">DTC_SERDESRXDisparityError to copy</param>
	DTC_SERDESRXDisparityError(const DTC_SERDESRXDisparityError& in) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="in">DTC_SERDESRXDisparityError rvalue</param>
	DTC_SERDESRXDisparityError(DTC_SERDESRXDisparityError&& in) = default;

	/// <summary>
	/// Default Copy Assignment Operator
	/// </summary>
	/// <param name="in">DTC_SERDESRXDisparityError to copy</param>
	/// <returns>DTC_SERDESRXDisparityError Reference</returns>
	DTC_SERDESRXDisparityError& operator=(const DTC_SERDESRXDisparityError& in) = default;
	/// <summary>
	/// Default Move Assignment Operator
	/// </summary>
	/// <param name="in">DTC_SERDESRXDisparityError rvalue</param>
	/// <returns>DTC_SERDESRXDisparityError reference</returns>
	DTC_SERDESRXDisparityError& operator=(DTC_SERDESRXDisparityError&& in) = default;

	/// <summary>
	/// Sets the data bits
	/// </summary>
	/// <param name="data">Data bits to set</param>
	void SetData(std::bitset<2> data) { data_ = data; }

	/// <summary>
	/// Gets the data bitset
	/// </summary>
	/// <returns>Data bits</returns>
	std::bitset<2> GetData() const { return data_; }

	/// <summary>
	/// Get the integer representation of the error flags
	/// </summary>
	/// <param name="output">Whether to output data (signature different than bitset version)</param>
	/// <returns>Data as int</returns>
	int GetData(bool output) const
	{
		if (output) return static_cast<int>(data_.to_ulong());
		return 0;
	}

	/// <summary>
	/// Write the DTC_SERDESRXDisparityError to stream in JSON format.
	/// Note that the JSON represents an object, calling code should add a key if there's other objects to stream
	/// </summary>
	/// <param name="stream">Stream to write to</param>
	/// <param name="error">DTC_SERDESRXDisparityError to parse</param>
	/// <returns>Stream object for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, DTC_SERDESRXDisparityError error)
	{
		stream << "{\"low\":" << error.GetData()[0] << ",\"high\":" << error.GetData()[1] << "}";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_SERDESRXDisparityError_h
