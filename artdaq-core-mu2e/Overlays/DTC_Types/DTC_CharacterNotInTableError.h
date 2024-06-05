#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_CharacterNotInTableError_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_CharacterNotInTableError_h

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"

#include <bitset>
#include <ostream>

namespace DTCLib {

/// <summary>
/// This structure is used to decode the SERDES Character Not In Table Error register
/// </summary>
class DTC_CharacterNotInTableError
{
	std::bitset<2> data_;

public:
	/// <summary>
	/// Default Constructor
	/// Initializes data bits to 0,0
	/// </summary>
	DTC_CharacterNotInTableError();
	/// <summary>
	/// Constructor using data bits
	/// </summary>
	/// <param name="data">data bits for this instance</param>
	explicit DTC_CharacterNotInTableError(std::bitset<2> data);
	/// <summary>
	/// DTC_CharacterNotInTableError Constructor
	/// </summary>
	/// <param name="data">Register value</param>
	/// <param name="link">Specific link to read</param>
	DTC_CharacterNotInTableError(uint32_t data, DTC_Link_ID link);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="in">DTC_CharacterNotInTableError to copy</param>
	DTC_CharacterNotInTableError(const DTC_CharacterNotInTableError& in) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="in">DTC_CharacterNotInTableError rvalue</param>
	DTC_CharacterNotInTableError(DTC_CharacterNotInTableError&& in) = default;

	/// <summary>
	/// Default Copy Assignment Operator
	/// </summary>
	/// <param name="in">DTC_CharacterNotInTableError to copy</param>
	/// <returns>DTC_CharacterNotInTableError reference</returns>
	DTC_CharacterNotInTableError& operator=(const DTC_CharacterNotInTableError& in) = default;
	/// <summary>
	/// Default Move Assignment Operator
	/// </summary>
	/// <param name="in">DTC_CharacterNotInTableError rvalue</param>
	/// <returns>DTC_CharacterNotInTableError reference</returns>
	DTC_CharacterNotInTableError& operator=(DTC_CharacterNotInTableError&& in) = default;

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
	/// Write the DTC_CharacterNotInTableError to stream in JSON format.
	/// Note that the JSON represents an object, calling code should add a key if there's other objects to stream
	/// </summary>
	/// <param name="stream">Stream to write to</param>
	/// <param name="error">DTC_CharacterNotInTableError to parse</param>
	/// <returns>Stream object for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, DTC_CharacterNotInTableError error)
	{
		stream << "{\"low\":" << error.GetData()[0] << ",\"high\":" << error.GetData()[1] << "}";
		return stream;
	}
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_CharacterNotInTableError_h
