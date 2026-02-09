#ifndef ARTDAQ_CORE_MU2E_DATA_MOBILESYNCDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_MOBILESYNCDATADECODER_HH

#include "artdaq-core-mu2e/Overlays/Decoders/DTCDataDecoder.hh"

#include <memory>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <cstdint>
#include <cstring>

namespace mu2e {
class MobileSyncDataDecoder : public DTCDataDecoder
{
public:
	explicit MobileSyncDataDecoder(DTCLib::DTC_SubEvent const& evt);

	// data packet always has 16 bytes
	static constexpr unsigned kPacketBytes = 16;
	static constexpr unsigned kPacketBits = kPacketBytes * 8;
	struct MobileSyncPacket
	{
		std::array<uint8_t, kPacketBytes> bytes{};
	};
	static_assert(sizeof(MobileSyncPacket) == kPacketBytes);

	uint8_t GetDataIdTag() const { return data_id_tag_; }
	const std::string& GetFirmwareGitTag() const { return git_tag_; }
	int GetPayloadVersion() const { return payload_version_; }

	unsigned GetNHitsPerPacket() const;
	bool GetHitTime(const MobileSyncPacket* packet, unsigned hit, double& time) const;
	bool GetHitTOT(const MobileSyncPacket* packet, unsigned hit, double& tot) const;

	typedef std::vector<MobileSyncPacket> sync_data_t;

	sync_data_t GetMobileSyncPackets(size_t blockIndex);

private:
	// dataset info
	uint8_t data_id_tag_{0};  // obtained from data header, maps to payload version and git tag
	int payload_version_{0};  // defines data in packets
	std::string git_tag_{"v0_00_00"};  // tag in sync-firmware (firmware version used in run)
	double clk_period_ns_{0};
	void SetGitTagAndPayloadVersion(uint8_t data_id_tag);  // maps data ID tag to firmware git tag
														   // and payload version
	void SetTimestampingClockPeriod(uint8_t data_id_tag);  // timestamping module clock period in ns

	// packets always filled with hits
	struct HitFormat
	{
		unsigned nHits, hitBits;
		unsigned coarseRisingEdgeTickOffset, coarseRisingEdgeTickNBits;
		unsigned coarseTicksHighOffset, coarseTicksHighNBits;
		unsigned realHitOffset, realHitNBits;  // sometimes inject fake hit to make packet 16 bytes
	};

	// payload version specific hit formatting
	static constexpr HitFormat kV0{2, 64, 0, 16, 0, 0, 0, 0};
	static constexpr HitFormat kV1{2, 64, 0, 16, 26, 16, 63, 1};

	// grab hit format based on payload version
	const HitFormat* HitPayloadFormat() const noexcept
	{
		switch (payload_version_)
		{
			case 0:
				return &kV0;
			case 1:
				return &kV1;
			default:
				return nullptr;
		}
	}

	// decoding functions
	bool GetField(const MobileSyncPacket* packet, unsigned field_offset, unsigned field_nbits,
				  unsigned hit, uint32_t& bits) const;
	bool ExtractBits(const MobileSyncPacket* packet, unsigned start_bit, unsigned nbits,
					 uint32_t& bits) const;
	bool GetCoarseRisingEdgeTick(const MobileSyncPacket* packet, unsigned hit,
								 uint32_t& bits) const;
	bool GetCoarseTicksHigh(const MobileSyncPacket* packet, unsigned hit, uint32_t& bits) const;
	bool HitIsReal(const MobileSyncPacket* packet, unsigned hit) const;
};
}  // namespace mu2e

#endif  // ARTDAQ_CORE_MU2E_DATA_MOBILESYNCDATADECODER_HH
