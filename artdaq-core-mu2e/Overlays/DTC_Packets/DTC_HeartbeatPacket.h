#ifndef artdaq_core_mu2e_Overlays_DTC_Packets_DTC_HeartbeatPacket_h
#define artdaq_core_mu2e_Overlays_DTC_Packets_DTC_HeartbeatPacket_h

#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DataPacket.h"
#include "artdaq-core-mu2e/Overlays/DTC_Packets/DTC_DMAPacket.h"

#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventMode.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_EventWindowTag.h"
#include "artdaq-core-mu2e/Overlays/DTC_Types/DTC_Link_ID.h"

#include <cstdint>
#include <string>

namespace DTCLib {

/// <summary>
/// The DTC Heartbeat Packet (sometimes referred to as a "Readout Request" packet)
/// </summary>
class DTC_HeartbeatPacket : public DTC_DMAPacket
{
public:
	/// <summary>
	/// Construct a DTC_HeartbeatPacket
	/// </summary>
	/// <param name="link">Destination Link</param>
	explicit DTC_HeartbeatPacket(DTC_Link_ID link);
	/// <summary>
	/// Construct a DTC_HeartbeatPacket
	/// </summary>
	/// <param name="link">Destination Link</param>
	/// <param name="event_tag">Timestamp of request</param>
	/// <param name="eventMode">Debug event mode bytes (Default: nullptr) If not null, must be 6 bytes long</param>
	/// <param name="deliveryRingTDC">TDC value from Delivery Ring</param>
	DTC_HeartbeatPacket(DTC_Link_ID link, DTC_EventWindowTag event_tag, DTC_EventMode eventMode = DTC_EventMode(), uint8_t deliveryRingTDC = 0);
	/// <summary>
	/// Default Copy Constructor
	/// </summary>
	/// <param name="right">DTC_HeartbeatPacket to copy</param>
	DTC_HeartbeatPacket(const DTC_HeartbeatPacket& right) = default;
	/// <summary>
	/// Default Move Constructor
	/// </summary>
	/// <param name="right">DTC_HeartbeatPacket to move</param>
	DTC_HeartbeatPacket(DTC_HeartbeatPacket&& right) = default;
	/// <summary>
	/// Construct a DTC_HeartbeatPacket from the given DTC_DataPacket
	/// </summary>
	/// <param name="in">DTC_DataPacket to overlay</param>
	explicit DTC_HeartbeatPacket(const DTC_DataPacket in);

	/// <summary>
	/// Default Destructor
	/// </summary>
	virtual ~DTC_HeartbeatPacket() noexcept = default;

	/// <summary>
	/// Get the DTC_EventWindowTag stored in the HeartbeatPacket
	/// </summary>
	/// <returns>Timestamp of Heartbeat</returns>
	DTC_EventWindowTag GetEventWindowTag() const { return event_tag_; }

	/// <summary>
	/// Get the Mode bytes from the Heartbeat packet
	/// </summary>
	/// <returns>5-byte array containing mode bytes</returns>
	virtual DTC_EventMode GetData() { return eventMode_; }

	/// <summary>
	/// Convert a DTC_HeartbeatPacket to DTC_DataPacket in "owner" mode
	/// </summary>
	/// <returns>DTC_DataPacket with DTC_HeartbeatPacket contents set</returns>
	DTC_DataPacket ConvertToDataPacket() const override;
	/// <summary>
	/// Convert the DTC_HeartbeatPacket to JSON representation
	/// </summary>
	/// <returns>JSON-formatted string representation of DTC_HeartbeatPacket</returns>
	std::string toJSON() override;
	/// <summary>
	/// Converts the DTC_HeartbeatPacket to "packet format" representation (See DTC_DataPacket::toPacketFormat())
	/// </summary>
	/// <returns>"packet format" string representation of DTC_HeartbeatPacket</returns>
	std::string toPacketFormat() override;

private:
	DTC_EventWindowTag event_tag_;
	DTC_EventMode eventMode_;
	uint8_t deliveryRingTDC_;
};

}  // namespace DTCLib

#endif  // artdaq_core_mu2e_Overlays_DTC_Packets_DTC_HeartbeatPacket_h
