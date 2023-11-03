#ifndef DTC_TYPES_H
#define DTC_TYPES_H

#include <bitset>   // std::bitset
#include <cstdint>  // uint8_t, uint16_t
#include <iomanip>
#include <vector>  // std::vector

#ifndef __CLING__
#include "TRACE/tracemf.h"
#else
class   TraceStreamer;
#define TLVL_WARNING 1
#define TLVL_INFO    2
#endif


namespace DTCLib {

typedef uint16_t roc_address_t;
typedef uint16_t roc_data_t;

enum DTC_Subsystem : uint8_t
{
	DTC_Subsystem_Tracker = 0,
	DTC_Subsystem_Calorimeter = 1,
	DTC_Subsystem_CRV = 2,
	DTC_Subsystem_Other = 3,
	DTC_Subsystem_STM = 4,
	DTC_Subsystem_ExtMon = 5,
};

enum DTC_Link_ID : uint8_t
{
	DTC_Link_0 = 0,
	DTC_Link_1 = 1,
	DTC_Link_2 = 2,
	DTC_Link_3 = 3,
	DTC_Link_4 = 4,
	DTC_Link_5 = 5,
	DTC_Link_CFO = 6,
	DTC_Link_EVB = 7,
	DTC_Link_Unused,
	DTC_Link_ALL = 255
};

#ifndef __CLING__
inline TraceStreamer& operator<<(TraceStreamer& ts, DTC_Link_ID const& link)
{
	return ts << static_cast<unsigned int>(link);
}
#endif

inline std::ostream& operator<<(std::ostream& o, DTC_Link_ID const& link)
{
	return o << static_cast<unsigned int>(link);
}

static const std::vector<DTC_Link_ID> DTC_Links{DTC_Link_0,
												DTC_Link_1,
												DTC_Link_2,
												DTC_Link_3,
												DTC_Link_4,
												DTC_Link_5};

enum DTC_PLL_ID : uint8_t
{
	DTC_PLL_Link_0 = 0,
	DTC_PLL_Link_1 = 1,
	DTC_PLL_Link_2 = 2,
	DTC_PLL_Link_3 = 3,
	DTC_PLL_Link_4 = 4,
	DTC_PLL_Link_5 = 5,
	DTC_PLL_CFO_RX = 6,
	DTC_PLL_CFO_TX = 7,
	DTC_PLL_EVB_TXRX = 8,
	DTC_PLL_PunchedClock = 9,
	DTC_PLL_Unused,
};
static const std::vector<DTC_PLL_ID> DTC_PLLs{DTC_PLL_Link_0,
											  DTC_PLL_Link_1,
											  DTC_PLL_Link_2,
											  DTC_PLL_Link_3,
											  DTC_PLL_Link_4,
											  DTC_PLL_Link_5};

enum DTC_OscillatorType
{
	DTC_OscillatorType_SERDES,
	DTC_OscillatorType_DDR,
	DTC_OscillatorType_Timing,
};

enum DTC_SerdesClockSpeed
{
	DTC_SerdesClockSpeed_25Gbps,
	DTC_SerdesClockSpeed_3125Gbps,
	DTC_SerdesClockSpeed_48Gbps,
	DTC_SerdesClockSpeed_Unknown
};

enum DTC_DebugType
{
	DTC_DebugType_SpecialSequence = 0,
	DTC_DebugType_ExternalSerial = 1,
	DTC_DebugType_ExternalSerialWithReset = 2,
	DTC_DebugType_RAMTest = 3,
	DTC_DebugType_DDRTest = 4,
	DTC_DebugType_Invalid = 5,
};

/// <summary>
/// The DTC_DebugTypeConverter converts a DTC_DebugType enumeration value to string or JSON representation
/// </summary>
struct DTC_DebugTypeConverter
{
	DTC_DebugType type_;  ///< DTC_DebugType to convert

	/// <summary>
	/// Construct a DTC_DebugTypeConverter instance using the given DTC_DebugType
	/// </summary>
	/// <param name="type">DTC_DebugType to convert</param>
	explicit DTC_DebugTypeConverter(DTC_DebugType type)
		: type_(type) {}

	/// <summary>
	/// Convert the DTC_DebugType to its string representation
	/// </summary>
	/// <returns>String representation of DTC_DebugType</returns>
	std::string toString() const
	{
		switch (type_)
		{
			case DTC_DebugType_SpecialSequence:
				return "Special Sequence";
			case DTC_DebugType_ExternalSerial:
				return "External Serial";
			case DTC_DebugType_ExternalSerialWithReset:
				return "External Serial with FIFO Reset";
			case DTC_DebugType_RAMTest:
				return "FPGA SRAM Error Checking";
			case DTC_DebugType_DDRTest:
				return "DDR3 Memory Error Checking";
			case DTC_DebugType_Invalid:
				return "INVALID!!!";
		}
		return "Unknown";
	}

	static DTC_DebugType ConvertToDebugType(std::string const& type);

	/// <summary>
	/// Write a DTC_DebugTypeConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="type">DTC_DebugTypeConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
#ifndef __CLING__
	friend std::ostream& operator<<(std::ostream& stream, const DTC_DebugTypeConverter& type)
	{
		stream << "\"DTC_DebugType\":\"" << type.toString() << "\"";
		return stream;
	}
#endif
};

enum DTC_RXBufferStatus
{
	DTC_RXBufferStatus_Nominal = 0,
	DTC_RXBufferStatus_BufferEmpty = 1,
	DTC_RXBufferStatus_BufferFull = 2,
	DTC_RXBufferStatus_Underflow = 5,
	DTC_RXBufferStatus_Overflow = 6,
	DTC_RXBufferStatus_Unknown = 0x10,
};

/// <summary>
/// The DTC_RXBufferStatusConverter converts a DTC_RXBufferStatus enumeration value to string or JSON representation
/// </summary>
struct DTC_RXBufferStatusConverter
{
	DTC_RXBufferStatus status_;  ///< DTC_RXBufferStatus to convert

	/// <summary>
	/// Construct a DTC_RXBufferStatusConverter instance using the given DTC_RXBufferStatus
	/// </summary>
	/// <param name="status">DTC_RXBufferStatus to convert</param>
	explicit DTC_RXBufferStatusConverter(DTC_RXBufferStatus status)
		: status_(status) {}

	/// <summary>
	/// Convert the DTC_RXBufferStatus to its string representation
	/// </summary>
	/// <returns>String representation of DTC_RXBufferStatus</returns>
	std::string toString() const
	{
		switch (status_)
		{
			case DTC_RXBufferStatus_Nominal:
				return "Nominal";
			case DTC_RXBufferStatus_BufferEmpty:
				return "BufferEmpty";
			case DTC_RXBufferStatus_BufferFull:
				return "BufferFull";
			case DTC_RXBufferStatus_Overflow:
				return "Overflow";
			case DTC_RXBufferStatus_Underflow:
				return "Underflow";
			case DTC_RXBufferStatus_Unknown:
			default:
				return "Unknown";
		}
	}

	/// <summary>
	/// Write a DTC_RXBufferStatusConverter in JSON format to the given stream
	/// </summary>
	/// <param name="stream">Stream to write</param>
	/// <param name="status">DTC_RXBufferStatusConverter to serialize</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& stream, const DTC_RXBufferStatusConverter& status)
	{
		stream << "\"DTC_RXBufferStatus\":\"" << status.toString() << "\"";
		return stream;
	}
};

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

/// <summary>
/// A DTC_WrongVersionException is thrown when an attempt to initialize a DTC is made with a certain firmware version
/// expected, and the firmware does not match that version
/// </summary>
class DTC_WrongVersionException : public std::runtime_error
{
public:
	/// <summary>
	/// A DTC_WrongVersionException is thrown when an attempt is made to construct a DTC packet with data that does not
	/// match the packet type
	/// </summary>
	/// <param name="expected">Expected firmware version string</param>
	/// <param name="encountered">Encountered firmware version string</param>
	DTC_WrongVersionException(std::string expected, std::string encountered)
		: std::runtime_error("DTCwrongVersionException: Unexpected firmware version encountered: " + encountered + " != " + expected + " (expected)") {}
// 	/// <summary>
// 	/// Describe the exception
// 	/// </summary>
// 	/// <returns>String describing the exception</returns>
// 	const char* what() const throw()
// 	{
// 		return what_.c_str();
// 	}

// private:
// 	std::string what_;
};

/// <summary>
/// A DTC_WrongPacketTypeException is thrown when an attempt to decode a DMA packet is made and the type in the DMA
/// header is different than the type of the packet expected
/// </summary>
class DTC_WrongPacketTypeException : public std::runtime_error
{
public:
	/// <summary>
	/// A DTC_WrongPacketTypeException is thrown when an attempt is made to construct a DTC packet with data that does not
	/// match the packet type
	/// </summary>
	/// <param name="expected">Expected packet type</param>
	/// <param name="encountered">Encountered packet type</param>
	DTC_WrongPacketTypeException(int expected, int encountered)
		: std::runtime_error("DTCWrongPacketTypeException: Unexpected packet type encountered: " + std::to_string(encountered) + " != " + std::to_string(expected) + " (expected)") {}
// 	/// <summary>
// 	/// Describe the exception
// 	/// </summary>
// 	/// <returns>String describing the exception</returns>
// 	const char* what() const throw()
// 	{
// 		return what_.c_str();
// 	}

// private:
// 	std::string what_;
};

/// <summary>
/// A DTC_WrongPacketSizeException is thrown when an attempt to decode a DataHeaderPacket and the packet count does not agree with the block size
/// </summary>
class DTC_WrongPacketSizeException : public std::runtime_error
{
public:
	/// <summary>
	/// A DTC_WrongPacketSizeException is thrown when an attempt is made to construct a DataHeaderPacket with inconsistent sizes
	/// </summary>
	/// <param name="expected">Expected block size</param>
	/// <param name="encountered">Encountered block size</param>
	DTC_WrongPacketSizeException(int expected, int encountered)
		: std::runtime_error("DTC_WrongPacketSizeException: Unexpected block size encountered: " + std::to_string(encountered) + " != " + std::to_string(expected) + " (expected)") {}
// 	/// <summary>
// 	/// Describe the exception
// 	/// </summary>
// 	/// <returns>String describing the exception</returns>
// 	const char* what() const throw()
// 	{
// 		return what_.c_str();
// 	}

// private:
// 	std::string what_;
};
/// <summary>
/// A DTC_IOErrorException is thrown when the DTC is not communicating when communication is expected
/// </summary>
class DTC_IOErrorException : public std::runtime_error
{
public:
	/// <summary>
	/// A DTC_IOErrorException is thrown when an attempt is made to read or write from the DTC, and an unexpected status
	/// results
	/// </summary>
	/// <param name="retcode">Return code from IO operation</param>
	DTC_IOErrorException(int retcode)
		: std::runtime_error(std::string("DTCIOErrorException: Unable to communicate with the DTC: Error Code: ") + std::to_string(retcode)) {}
	DTC_IOErrorException(const std::string& errorMessage)
		: std::runtime_error(std::string("DTCIOErrorException: ") + errorMessage) {}
	
// 	/// <summary>
// 	/// Describe the exception
// 	/// </summary>
// 	/// <returns>String describing the exception</returns>
// 	const char* what() const throw()
// 	{
// 		return what_.c_str();
// 	}

// private:
// 	std::string what_;
};

/// <summary>
/// A DTC_DataCorruptionException is thrown when corrupt data is detected coming from the DTC
/// </summary>
class DTC_DataCorruptionException : public std::runtime_error
{
public:
	DTC_DataCorruptionException() 
		: std::runtime_error("DTCDataCorruptionException: Corruption detected in data stream from DTC")
	{}
	// /// <summary>
	// /// Describe the exception
	// /// </summary>
	// /// <returns>String describing the exception</returns>
	// const char* what() const throw() { return "DTCDataCorruptionException: Corruption detected in data stream from DTC"; }
};

/// <summary>
/// The mu2e event_tag is a 48-bit quantity. This class manages all the different ways it could be accessed.
/// </summary>
class DTC_EventWindowTag
{
	uint64_t event_tag_ : 48;

public:
	/// <summary>
	/// Default Constructor. Initializes Event Window Tag to value 0
	/// </summary>
	DTC_EventWindowTag();
	/// <summary>
	/// Construct a Event Window Tag using the given quad word
	/// </summary>
	/// <param name="event_tag">64-bit unsigned integer representing Event Window Tag. Top 16 bits will be discarded</param>
	explicit DTC_EventWindowTag(const uint64_t event_tag);
	/// <summary>
	/// Construct a Event Window Tag using the given low and high words
	/// </summary>
	/// <param name="event_tag_low">Lower 32 bits of Event Window Tag</param>
	/// <param name="event_tag_high">Upper 16 bits of Event Window Tag</param>
	DTC_EventWindowTag(const uint32_t event_tag_low, const uint16_t event_tag_high);
	/// <summary>
	/// Construct a DTC_EventWindowTag using the given byte array. Length of the array must be greater than 6 + offset!
	/// </summary>
	/// <param name="timeArr">Byte array to read event_tag from (i.e. DTC_DataPacket::GetData())</param>
	/// <param name="offset">Location of event_tag byte 0 in array (Default 0)</param>
	explicit DTC_EventWindowTag(const uint8_t* timeArr, int offset = 0);
	/// <summary>
	/// Construct a DTC_EventWindowTag using a std::bitset of the 48-bit event_tag
	/// </summary>
	/// <param name="event_tag">std::bitset containing event_tag</param>
	explicit DTC_EventWindowTag(const std::bitset<48> event_tag);
	/// <summary>
	/// Default copy constructor
	/// </summary>
	/// <param name="r">DTC_EventWindowTag to copy</param>
	DTC_EventWindowTag(const DTC_EventWindowTag& r) = default;
	/// <summary>
	/// Default move constructor
	/// </summary>
	/// <param name="r">DTC_EventWindowTag rvalue</param>
	DTC_EventWindowTag(DTC_EventWindowTag&& r) = default;

	/// <summary>
	/// DTC_EventWindowTag Default Destructor
	/// </summary>
	virtual ~DTC_EventWindowTag() = default;
	/// <summary>
	/// Default move assignment operator
	/// </summary>
	/// <param name="r">DTC_EventWindowTag rvalue</param>
	/// <returns>DTC_EventWindowTag reference</returns>
	DTC_EventWindowTag& operator=(DTC_EventWindowTag&& r) = default;
	/// <summary>
	/// Default copy assignment operator
	/// </summary>
	/// <param name="r">DTC_EventWindowTag to copy</param>
	/// <returns>DTC_EventWindowTag reference</returns>
	DTC_EventWindowTag& operator=(const DTC_EventWindowTag& r) = default;

	/// <summary>
	/// Compare two DTC_EventWindowTag instances
	/// </summary>
	/// <param name="r">Other event_tag</param>
	/// <returns>Result of comparison</returns>
	bool operator==(const DTC_EventWindowTag r) const { return r.GetEventWindowTag(true) == event_tag_; }

	/// <summary>
	/// Compare two DTC_EventWindowTag instances
	/// </summary>
	/// <param name="r">Other event_tag</param>
	/// <returns>Result of comparison</returns>
	bool operator!=(const DTC_EventWindowTag r) const { return r.GetEventWindowTag(true) != event_tag_; }

	/// <summary>
	/// Compare two DTC_EventWindowTag instances
	/// </summary>
	/// <param name="r">Other event_tag</param>
	/// <returns>Result of comparison</returns>
	bool operator<(const DTC_EventWindowTag r) { return r.GetEventWindowTag(true) > event_tag_; }

	/// <summary>
	/// Compare two DTC_EventWindowTag instances
	/// </summary>
	/// <param name="r">Other event_tag</param>
	/// <returns>Result of comparison</returns>
	bool operator<(const DTC_EventWindowTag r) const { return r.GetEventWindowTag(true) > event_tag_; }

	/// <summary>
	/// Add an integer to a event_tag instance
	/// </summary>
	/// <param name="r">Integer to add to event_tag</param>
	/// <returns>New event_tag with result</returns>
	DTC_EventWindowTag operator+(const int r) const { return DTC_EventWindowTag(r + event_tag_); }

	/// <summary>
	/// Set the Event Window Tag using the given quad word
	/// </summary>
	/// <param name="event_tag">64-bit unsigned integer representing event_tag. Top 16 bits will be discarded</param>
	void SetEventWindowTag(uint64_t event_tag) { event_tag_ = event_tag & 0x0000FFFFFFFFFFFF; }

	/// <summary>
	/// Set the Event Window Tag using the given low and high words
	/// </summary>
	/// <param name="event_tag_low">Lower 32 bits of the Event Window Tag</param>
	/// <param name="event_tag_high">Upper 16 bits of the timstamp</param>
	void SetEventWindowTag(uint32_t event_tag_low, uint16_t event_tag_high);

	/// <summary>
	/// Returns the timstamp as a 48-bit std::bitset
	/// </summary>
	/// <returns>the Event Window Tag as a 48-bit std::bitset</returns>
	std::bitset<48> GetEventWindowTag() const { return event_tag_; }

	/// <summary>
	/// Returns the Event Window Tag as a 64-bit unsigned integer
	/// </summary>
	/// <param name="dummy">Whether to return a event_tag (used to distinguish signature)</param>
	/// <returns>event_tag as a 64-bit unsigned integer</returns>
	uint64_t GetEventWindowTag(bool dummy) const
	{
		if (dummy)
		{
			return event_tag_;
		}
		return 0;
	}

	/// <summary>
	/// Copies the Event Window Tag into the given byte array, starting at some offset.
	/// Size of input array MUST be larger than offset + 6
	/// </summary>
	/// <param name="timeArr">Byte array for output</param>
	/// <param name="offset">Target index of byte 0 of the Event Window Tag</param>
	void GetEventWindowTag(const uint8_t* timeArr, int offset = 0) const;

	/// <summary>
	/// Convert the Event Window Tag to a JSON representation
	/// </summary>
	/// <param name="arrayMode">(Default: false) If true, will create a JSON array of the 6 bytes. Otherwise, represents
	/// event_tag as a single number</param> <returns>JSON-formatted string containing event_tag</returns>
	std::string toJSON(bool arrayMode = false) const;

	/// <summary>
	/// Convert the 48-bit event_tag to the format used in the Packet format definitions.
	/// Byte 1 | Byte 0
	/// Byte 3 | Byte 2
	/// Byte 5 | Byte 4
	/// </summary>
	/// <returns>String representing event_tag in "packet format"</returns>
	std::string toPacketFormat() const;
};

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

enum DTC_IICSERDESBusAddress : uint8_t
{
	DTC_IICSERDESBusAddress_EVB = 0x55,
	DTC_IICSERDESBusAddress_CFO = 0x5d,
	DTC_IICSERDESBusAddress_JitterAttenuator = 0x68,
};

enum DTC_IICDDRBusAddress : uint8_t
{
	DTC_IICDDRBusAddress_DDROscillator = 0x59,
};

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

struct DTC_EventMode
{
	uint8_t mode0{0};
	uint8_t mode1{0};
	uint8_t mode2{0};
	uint8_t mode3{0};
	uint8_t mode4{0};

	void GetEventMode(const uint8_t* arr, size_t start = 0) const
	{
		const_cast<uint8_t*>(arr)[start] = mode0;
		const_cast<uint8_t*>(arr)[start + 1] = mode1;
		const_cast<uint8_t*>(arr)[start + 2] = mode2;
		const_cast<uint8_t*>(arr)[start + 3] = mode3;
		const_cast<uint8_t*>(arr)[start + 4] = mode4;
	}
};

struct DTC_EVBStatus
{
	bool error;
	std::bitset<7> flags;

	enum class Flags : uint8_t
	{
		kEventFragmentTimeout = 0,
		kReserved1 = 1,
		kReserved2 = 2,
		kReserved3 = 3,
		kReserved4 = 4,
		kReserved5 = 5,
		kReserved6 = 6,
		kInvalid
	};

	DTC_EVBStatus()
		: error(false), flags(0) {}
	DTC_EVBStatus(uint8_t word)
	{
		error = (word & 0x80) != 0;
		flags = std::bitset<7>(word);
	}
};

struct DTC_LinkStatus
{
	bool error;
	std::bitset<7> flags;

	enum class Flags : uint8_t
	{
		kROCTimeoutError = 0,
		kReserved1 = 1,
		kPacketSequenceNumberError = 2,
		kPacketCRCError = 3,
		kReserved4 = 4,
		kReserved5 = 5,
		kFatalError = 6,
		kInvalid
	};
	DTC_LinkStatus()
		: error(false), flags(0) {}
	DTC_LinkStatus(uint8_t word)
	{
		error = (word & 0x80) != 0;
		flags = std::bitset<7>(word);
	}
};

/// <summary>
/// Several useful data manipulation utilities
/// </summary>
struct Utilities
{
	/// <summary>
	/// Create a string with "[value] [unit]" for a given number of bytes, using FormatBytes(bytes)
	/// </summary>
	/// <param name="bytes">Number of bytes to convert to string</param>
	/// <param name="extraUnit">Extra units to be applied after converted byte unit (i.e. "/s")</param>
	/// <returns>String with converted value</returns>
	static std::string FormatByteString(double bytes, std::string extraUnit);
	/// <summary>
	/// Determine the best units for describing a given number of bytes.
	/// Algorithm will divide by 1024, and if the result is greater than 1 and less than 1024, use that unit.
	/// Maximum unit is TB.
	/// </summary>
	/// <param name="bytes">Number of bytes to format</param>
	/// <returns>Pair of Value in "best unit" and string representation of unit (i.e. "KB", "MB", etc)</returns>
	static std::pair<double, std::string> FormatBytes(double bytes);
	/// <summary>
	/// Create a string with "[value] [unit]" for a given number of seconds, using FormatTime(seconds)
	/// </summary>
	/// <param name="seconds">Number of seconds to convert to string</param>
	/// <returns>String with converted value</returns>
	static std::string FormatTimeString(double seconds);
	/// <summary>
	/// Determine the best units for describing a given number of seconds.
	/// Will find largest time span which has unit greater than 1, up to days, down to ns.
	/// </summary>
	/// <param name="seconds">Number of seconds to convert</param>
	/// <returns>Pair of Value in "best unit" and string representation of unit (i.e. "ns", "us", "hours", etc)</returns>
	static std::pair<double, std::string> FormatTime(double seconds);

	/// <summary>
	/// Print out the buffer in hexdump -c format
	/// </summary>
	/// <param name="ptr">Pointer to the buffer</param>
	/// <param name="sz">Size of the buffer</param>
	/// <param name="quietCount">Number of lines to print at the begin/end. Default is 0, which prints entire buffer</param>
	/// <param name="tlvl">TLVL to use for printing (Default TLVL_INFO)</param>
	static void PrintBuffer(const void* ptr, size_t sz, size_t quietCount = 0, int tlvl = TLVL_INFO);

	static unsigned getOptionValue(int* index, char** argv[]);
	static unsigned long long getOptionValueLong(int* index, char** argv[]);

	static std::string getOptionString(int* index, char** argv[]);

	static unsigned getLongOptionValue(int* index, char** argv[]);
	static unsigned long long getLongOptionValueLong(int* index, char** argv[]);

	static std::string getLongOptionOption(int* index, char** argv[]);
	static std::string getLongOptionString(int* index, char** argv[]);
};
}  // namespace DTCLib

#endif  // DTC_TYPES_H
