#include "artdaq-core-mu2e/Overlays/Decoders/MobileSyncDataDecoder.hh"

#include "TRACE/tracemf.h"

#include <vector>

namespace mu2e {
MobileSyncDataDecoder::MobileSyncDataDecoder(DTCLib::DTC_SubEvent const& evt) : DTCDataDecoder(evt)
{
	if (block_count() > 0)
	{
		auto dataPtr = dataAtBlockIndex(0);
		auto hdr = dataPtr->GetHeader();
		data_id_tag_ = hdr->GetVersion();
		SetGitTagAndPayloadVersion(data_id_tag_);
		SetTimestampingClockPeriod(data_id_tag_);
	}
}

void MobileSyncDataDecoder::SetGitTagAndPayloadVersion(uint8_t data_id_tag)
{
	switch (data_id_tag)
	{
		case 0x00:
			payload_version_ = 0;
			git_tag_ = "v1_00_00";
			break;
		case 0x01:
			payload_version_ = 1;
			git_tag_ = "v2_00_00";
			break;
		default:
			payload_version_ = -1;
			git_tag_ = "unknown";
			break;
	}
}

void MobileSyncDataDecoder::SetTimestampingClockPeriod(uint8_t data_id_tag)
{
	switch (data_id_tag)
	{
		default:
			clk_period_ns_ = 5.0;
			break;
	}
}

unsigned MobileSyncDataDecoder::GetNHitsPerPacket() const
{
	const HitFormat* hf = HitPayloadFormat();
	return hf ? hf->nHits : 0;
}

bool MobileSyncDataDecoder::GetField(const MobileSyncPacket* packet, unsigned field_offset,
									 unsigned field_nbits, unsigned hit, uint32_t& bits) const
{
	if (!packet) return false;
	const HitFormat* hf = HitPayloadFormat();
	if (!hf) return false;
	if (hit >= hf->nHits) return false;

	const unsigned start_bit = field_offset + hit * hf->hitBits;
	return ExtractBits(packet, start_bit, field_nbits, bits);
}

bool MobileSyncDataDecoder::ExtractBits(const MobileSyncPacket* packet, unsigned start_bit,
										unsigned nbits, uint32_t& bits) const
{
	if (!packet) return false;
	if (nbits == 0 || nbits > 32 || start_bit + nbits > kPacketBits) return false;

	bits = 0;
	for (unsigned i = 0; i < nbits; i++)
	{
		unsigned bit = start_bit + i;
		unsigned byte = bit >> 3;
		unsigned bitInByte = bit & 0b111;
		bits |= (uint32_t)(((packet->bytes[byte] >> bitInByte) & 0b1) << i);
	}
	return true;
}

bool MobileSyncDataDecoder::GetCoarseRisingEdgeTick(const MobileSyncPacket* packet, unsigned hit,
													uint32_t& bits) const
{
	const HitFormat* hf = HitPayloadFormat();
	if (!hf) return false;
	return GetField(packet, hf->coarseRisingEdgeTickOffset, hf->coarseRisingEdgeTickNBits, hit,
					bits);
}

bool MobileSyncDataDecoder::GetCoarseTicksHigh(const MobileSyncPacket* packet, unsigned hit,
											   uint32_t& bits) const
{
	const HitFormat* hf = HitPayloadFormat();
	if (!hf) return false;
	return GetField(packet, hf->coarseTicksHighOffset, hf->coarseTicksHighNBits, hit, bits);
}

bool MobileSyncDataDecoder::HitIsReal(const MobileSyncPacket* packet, unsigned hit) const
{
	const HitFormat* hf = HitPayloadFormat();
	if (!hf) return false;

	uint32_t real = 0;
	return GetField(packet, hf->realHitOffset, hf->realHitNBits, hit, real) && (real != 0);
}

bool MobileSyncDataDecoder::GetHitTime(const MobileSyncPacket* packet, unsigned hit,
									   double& time) const
{
	if (!packet) return false;

	uint32_t rising_edge_tick;
	switch (payload_version_)
	{
		case 0:
			if (GetCoarseRisingEdgeTick(packet, hit, rising_edge_tick))  // real flag not in v0
			{
				// v0 padded event end with 0x0, and started event with 0xdead
				if (rising_edge_tick == (uint32_t)(0xDEAD) || rising_edge_tick == 0) return false;
				time = rising_edge_tick * clk_period_ns_;
				return true;
			}
			else
				return false;
		case 1:
			if (GetCoarseRisingEdgeTick(packet, hit, rising_edge_tick) && HitIsReal(packet, hit))
			{
				time = rising_edge_tick * clk_period_ns_;
				return true;
			}
			else
				return false;
		default:
			return false;
	}
}

bool MobileSyncDataDecoder::GetHitTOT(const MobileSyncPacket* packet, unsigned hit,
									  double& tot) const
{
	if (!packet) return false;

	uint32_t ticks_high;
	switch (payload_version_)
	{
		case 1:
			if (GetCoarseTicksHigh(packet, hit, ticks_high) && HitIsReal(packet, hit))
			{
				tot = ticks_high * clk_period_ns_;
				return true;
			}
			else
				return false;
		default:
			return false;
	}
}

MobileSyncDataDecoder::sync_data_t MobileSyncDataDecoder::GetMobileSyncPackets(
	size_t blockIndex) const
{
	sync_data_t output;

	auto dataPtr = dataAtBlockIndex(blockIndex);
	if (dataPtr == nullptr) return output;

	const auto nPackets = dataPtr->GetHeader()->GetPacketCount();
	output.resize(nPackets);

	std::memcpy(output.data(), dataPtr->GetData(), nPackets * sizeof(MobileSyncPacket));

	return output;
}

}  // namespace mu2e

