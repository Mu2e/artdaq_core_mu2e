#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSOperationType_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSOperationType_h

#include <cstdint>
#include <ostream>
#include <string>

namespace DTCLib {

/// <summary>
/// Possible values for the Op word of the DCS Request packet.
/// </summary>
enum DTC_DCSOperationType : uint8_t
{
	DTC_DCSOperationType_Read = 0,
	DTC_DCSOperationType_Write = 1,
	DTC_DCSOperationType_BlockRead = 2,
	DTC_DCSOperationType_BlockWrite = 3,
	DTC_DCSOperationType_DoubleRead = 4,
	DTC_DCSOperationType_DoubleWrite = 5,
	DTC_DCSOperationType_InvalidS2C = 0xC, //1100 would otherwise be nonsense "ack of double read"
	DTC_DCSOperationType_Timeout = 0xE, //1110 would otherwise be nonsense "ack of double block read"
	DTC_DCSOperationType_Unknown = 0xF //reserved for software use as 'Unknown,' would otherwise be nonsense "ack of double block write"
};

/// <summary>
/// Convert a DTC_DCSOperationType enumeration value to its string or JSON representation
/// </summary>
struct DTC_DCSOperationTypeConverter
{
	DTC_DCSOperationType type_;  ///< DTC_DCSOperationType to convert

	/// <summary>
	/// DTC_DCSOperationTypeConverter Constructor
	/// </summary>
	/// <param name="type">DTC_DCSOperationType to convert</param>
	explicit DTC_DCSOperationTypeConverter(DTC_DCSOperationType type)
		: type_(type) {}

	/// <summary>
	/// Convert the type to its string representation
	/// </summary>
	/// <returns></returns>
	std::string toString() const
	{
		switch (type_)
		{
			case DTC_DCSOperationType_Read:
				return "Read";
			case DTC_DCSOperationType_Write:
				return "Write";
			case DTC_DCSOperationType_BlockRead:
				return "BlockRead";
			case DTC_DCSOperationType_BlockWrite:
				return "BlockWrite";
			case DTC_DCSOperationType_DoubleRead:
				return "DoubleRead";
			case DTC_DCSOperationType_DoubleWrite:
				return "DoubleWrite";
			case DTC_DCSOperationType_Unknown:
			default:
				return "Unknown";
		}
	}

	/// <summary>
	/// Write a DTC_DCSOperationTypeConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="type">DTC_DCSOperationTypeConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_DCSOperationTypeConverter& type)
	{
		switch (type.type_)
		{
			case DTC_DCSOperationType_Read:
				stream << "\"Read\"";
				break;
			case DTC_DCSOperationType_Write:
				stream << "\"Write\"";
				break;
			case DTC_DCSOperationType_BlockRead:
				stream << "\"BlockRead\"";
				break;
			case DTC_DCSOperationType_BlockWrite:
				stream << "\"BlockWrite\"";
				break;
			case DTC_DCSOperationType_DoubleRead:
				stream << "\"DoubleRead\"";
				break;
			case DTC_DCSOperationType_DoubleWrite:
				stream << "\"DoubleWrite\"";
				break;
			case DTC_DCSOperationType_Unknown:
			default:
				stream << "\"Unknown\"";
				break;
		}
		return stream;
	}
};


}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DCSOperationType_h
