#ifndef artdaq_core_mu2e_Overlays_DTC_Types_DTC_EventWindowTag_h
#define artdaq_core_mu2e_Overlays_DTC_Types_DTC_EventWindowTag_h

#include <bitset>
#include <cstdint>
#include <string>

namespace DTCLib {

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

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Types_DTC_EventWindowTag_h
