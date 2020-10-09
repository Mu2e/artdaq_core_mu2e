
#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_CRVFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_CRVFRAGMENT_HH

#include "mu2e-artdaq-core/Overlays/ArtFragment.hh"
#include <memory>
#include <vector>

namespace mu2e {
class CRVFragment : public ArtFragment
{
public:
	explicit CRVFragment(artdaq::Fragment const& f)
		: ArtFragment(f) {}

	struct CRVROCStatusPacket
	{
		uint8_t unused1 : 4;
		uint8_t PacketType : 4;  // == 0x06
		uint8_t ControllerID;

		uint16_t ControllerEventWordCount;

		uint8_t ActiveFEBFlags2;
		uint8_t unused2;

		uint8_t ActiveFEBFlags0;
		uint8_t ActiveFEBFlags1;

		uint8_t unused3;
		uint8_t unused4;

		uint16_t TriggerCount;

		uint8_t unused5;
		uint8_t unused6;

		uint8_t Errors;
		uint8_t EventType;

		CRVROCStatusPacket()
			: unused1(0)
			, PacketType(0)
			, ControllerID(0)
			, ControllerEventWordCount(0)
			, ActiveFEBFlags2(0)
			, unused2(0)
			, ActiveFEBFlags0(0)
			, ActiveFEBFlags1(0)
			, unused3(0)
			, unused4(0)
			, TriggerCount(0)
			, unused5(0)
			, unused6(0)
			, Errors(0)
			, EventType(0)
		{}
	};

	struct CRVHitReadoutPacket
	{
		uint16_t SiPMID;

		uint16_t HitTime : 10;
		uint16_t NumSamples : 6;

		uint8_t WaveformSample0;
		uint8_t WaveformSample1;

		uint8_t WaveformSample2;
		uint8_t WaveformSample3;

		uint8_t WaveformSample4;
		uint8_t WaveformSample5;

		uint8_t WaveformSample6;
		uint8_t WaveformSample7;

		CRVHitReadoutPacket()
			: SiPMID(0)
			, HitTime(0)
			, NumSamples(0)
			, WaveformSample0(0)
			, WaveformSample1(0)
			, WaveformSample2(0)
			, WaveformSample3(0)
			, WaveformSample4(0)
			, WaveformSample5(0)
			, WaveformSample6(0)
			, WaveformSample7(0) {}
		std::array<unsigned int, 8> Waveform() const
		{
			std::array<unsigned int, 8> output;
			output[0] = WaveformSample0;
			output[1] = WaveformSample1;
			output[2] = WaveformSample2;
			output[3] = WaveformSample3;
			output[4] = WaveformSample4;
			output[5] = WaveformSample5;
			output[6] = WaveformSample6;
			output[7] = WaveformSample7;
			return output;
		}
	};

	std::unique_ptr<CRVROCStatusPacket> GetCRVROCStatusPacket(size_t blockIndex) const;
	std::vector<CRVHitReadoutPacket> GetCRVHitReadoutPackets(size_t blockIndex) const;
};
}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_CRVFRAGMENT_HH