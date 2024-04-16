#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataPacket_h

#include <cstdint>
#include <cstring>
#include <ostream>
#include <string>
#include <vector>

namespace DTCLib {

/// <summary>
/// The DTC_DataPacket class represents the 16 bytes of raw data for all DTC packets.
/// The class works in two modes: "overlay" mode, where the data is in a fixed location in memory and modification is
/// restricted, and "owner" mode, where the DataPacket is a concrete instance.
/// </summary>
class DTC_DataPacket
{
public:
	/// <summary>
	/// Construct a DTC_DataPacket in owner mode
	/// </summary>
	DTC_DataPacket();

	/// <summary>
	/// Construct a DTC_DataPacket using a pointer to data. Flag will be set that the packet is read-only.
	/// </summary>
	/// <param name="data">Pointer to data</param>
	explicit DTC_DataPacket(const void* data)
		: dataPtr_(static_cast<const uint8_t*>(data)), dataSize_(16), memPacket_(true) {}

	/// <summary>
	/// Creates a copy of the DTC_DataPacket. Mode is preserved, if the existing DataPacket was in "owner" mode, a deep
	/// copy is made, otherwise the reference to the read-only memory will be copied.
	/// </summary>
	/// <param name="in">Input DTC_DataPacket</param>
	DTC_DataPacket(const DTC_DataPacket& in);
	/// <summary>
	/// Default move constructor
	/// </summary>
	/// <param name="in">DTC_DataPacket rvalue</param>
	DTC_DataPacket(DTC_DataPacket&& in) = default;

	virtual ~DTC_DataPacket();

	/// <summary>
	/// Default copy-assignment operator
	/// </summary>
	/// <param name="in">DTC_DataPacket lvalue</param>
	/// <returns>DTC_DataPacket reference</returns>
	DTC_DataPacket& operator=(const DTC_DataPacket& in) = default;
	/// <summary>
	/// Default move-assignment operator
	/// </summary>
	/// <param name="in">DTC_DataPacket rvalue</param>
	/// <returns>DTC_DataPacket reference</returns>
	DTC_DataPacket& operator=(DTC_DataPacket&& in) = default;

	/// <summary>
	/// Set the given byte of the DataPacket.
	/// No-op if the DataPacket is in overlay mode
	/// </summary>
	/// <param name="index">Index of the byte to change</param>
	/// <param name="data">Value of the byte</param>
	void SetByte(uint16_t index, uint8_t data);
	/// <summary>
	/// Gets the current value of the given byte
	/// </summary>
	/// <param name="index">Index of the byte</param>
	/// <returns>Value of the byte</returns>
	uint8_t GetByte(uint16_t index) const;
	/// <summary>
	/// Creates a JSON representation of the DTC_DataPacket
	/// </summary>
	/// <returns>JSON-formatted string representation of the DTC_DataPacket</returns>
	std::string toJSON() const;
	/// <summary>
	/// Create a "packet format" representation of the DTC_DataPacket. See "DTC Hardware User's Guide" for "packet format"
	/// representation.
	/// </summary>
	/// <returns>"packet format" string representation of the DTC_DataPacket</returns>
	std::string toPacketFormat() const;
	/// <summary>
	/// Resize a DTC_DataPacket in "owner" mode. New size must be larger than current.
	/// </summary>
	/// <param name="dmaSize">Size in bytes of the new packet</param>
	/// <returns>If the resize operation was successful</returns>
	bool Resize(const uint16_t dmaSize);

	/// <summary>
	/// Get the current size, in bytes, of the DTC_DataPacket (default: 16)
	/// </summary>
	/// <returns></returns>
	uint16_t GetSize() const { return dataSize_; }

	/// <summary>
	/// Determine whether the DataPacket is in owner mode or overlay mode
	/// </summary>
	/// <returns>True if the DataPacket is in overlay mode</returns>
	bool IsMemoryPacket() const { return memPacket_; }

	/// <summary>
	/// Add a DTC_DataPacket's data to this DataPacket. DataPacket must be large enough to accomodate data and in "owner"
	/// mode.
	/// </summary>
	/// <param name="other">Packet to add</param>
	/// <param name="offset">Where to copy packet in this DataPacket</param>
	/// <returns>True if successful</returns>
	bool CramIn(DTC_DataPacket& other, int offset)
	{
		if (other.dataSize_ + offset <= dataSize_)
		{
			memcpy(const_cast<uint8_t*>(dataPtr_) + offset, other.dataPtr_, other.dataSize_);
			return true;
		}
		return false;
	}

	/// <summary>
	/// Gets the pointer to the data
	/// </summary>
	/// <returns>Pointer to DTC_DataPacket data. Use GetSize() to determine the valid range of this pointer</returns>
	const uint8_t* GetData() const { return dataPtr_; }

	/// <summary>
	/// Comparison operator. Returns this.Equals(other)
	/// </summary>
	/// <param name="other">DataPacket to compare</param>
	/// <returns>this.Equals(other)</returns>
	bool operator==(const DTC_DataPacket& other) const { return Equals(other); }

	/// <summary>
	/// Comparison operator. Returns !this.Equals(other)
	/// </summary>
	/// <param name="other">DataPacket to compare</param>
	/// <returns>!this.Equals(other)</returns>
	bool operator!=(const DTC_DataPacket& other) const { return !Equals(other); }

	/// <summary>
	/// Compare the contents of two DataPackets. Ignores the first two bytes as they may differ (reserved words in most
	/// DMA packets).
	/// </summary>
	/// <param name="other">Data packet to compare</param>
	/// <returns>Whether the two DataPackets contents are equal</returns>
	bool Equals(const DTC_DataPacket& other) const;

	/// <summary>
	/// Serialize a DTC_DataPacket to the given ostream
	/// </summary>
	/// <param name="s">Stream to write to</param>
	/// <param name="p">DataPacket to stream, in binary</param>
	/// <returns>Stream reference for continued streaming</returns>
	friend std::ostream& operator<<(std::ostream& s, DTC_DataPacket& p)
	{
		return s.write(reinterpret_cast<const char*>(p.dataPtr_), p.dataSize_);
	}

private:
	const uint8_t* dataPtr_;
	uint16_t dataSize_;
	bool memPacket_;
	std::vector<uint8_t> vals_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_DataPacket_h
