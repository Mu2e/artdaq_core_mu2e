#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_Exceptions_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_Exceptions_h

#include <stdexcept>

namespace DTCLib {

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

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_Exceptions_h
