#ifndef mu2e_artdaq_core_Overlays_ArtFragmentReader_hh
#define mu2e_artdaq_core_Overlays_ArtFragmentReader_hh

////////////////////////////////////////////////////////////////////////
// ArtFragmentReader
//
// Class derived from mu2eFragment which provides additional methods
// for accessing data stored in a mu2eFragment
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "mu2e-artdaq-core/Overlays/ArtFragment.hh"
//#include "RecoDataProducts/CrvDigi.hh"

#include <iostream>

namespace mu2e {
class ArtFragmentReader;

std::ostream &operator<<(std::ostream &, ArtFragmentReader const &);
}  // namespace mu2e

class mu2e::ArtFragmentReader : public mu2e::ArtFragment
{
public:
	ArtFragmentReader(artdaq::Fragment const &f)
		: ArtFragment(f){};

	// DataBlock Header Accessor Methods (by block address)
	adc_t DBH_ByteCount(adc_t const *pos);
	bool DBH_Valid(adc_t const *pos);
	adc_t DBH_ROCID(adc_t const *pos);
	adc_t DBH_PacketType(adc_t const *pos);
	adc_t DBH_PacketCount(adc_t const *pos);
	uint64_t DBH_Timestamp(adc_t const *pos);
	adc_t DBH_TimestampLow(adc_t const *pos);
	adc_t DBH_TimestampMedium(adc_t const *pos);
	adc_t DBH_TimestampHigh(adc_t const *pos);
	adc_t DBH_Status(adc_t const *pos);
	adc_t DBH_FormatVersion(adc_t const *pos);
	adc_t DBH_EVBMode(adc_t const *pos);
	adc_t DBH_SubsystemID(adc_t const *pos);
	adc_t DBH_DTCID(adc_t const *pos);

	// TRK DataBlock Payload Accessor Methods (by block address)
	adc_t DBT_StrawIndex(adc_t const *pos);
	adc_t DBT_TDC0(adc_t const *pos);
	adc_t DBT_TDC1(adc_t const *pos);
	adc_t DBT_TOT0(adc_t const *pos);
	adc_t DBT_TOT1(adc_t const *pos);
	std::array<adc_t, 15> DBT_Waveform(adc_t const *pos);
	adc_t DBT_Flags(adc_t const *pos);

	// CAL DataBlock Payload Accessor Method (by block address)
	adc_t DBCR_NumHits(adc_t const *pos);

	adc_t DBC_ChannelID(adc_t const *pos, size_t hitIdx);
	adc_t DBC_BoardID(adc_t const *pos, size_t hitIdx);
	adc_t DBC_Time(adc_t const *pos, size_t hitIdx);
	adc_t DBC_NumSamples(adc_t const *pos, size_t hitIdx);
	adc_t DBC_PeakSampleIdx(adc_t const *pos, size_t hitIdx);
        std::vector<int> DBC_Waveform(adc_t const *pos, size_t hitIdx);

	adc_t DBC_DIRACOutputA(adc_t const *pos, size_t hitIdx);
	adc_t DBC_DIRACOutputB(adc_t const *pos, size_t hitIdx);
	adc_t DBC_ErrorFlags(adc_t const *pos, size_t hitIdx);

	// CRV ROC Status Accessor Methods (by block address)
	adc_t DBVR_ControllerID(adc_t const *pos);
	adc_t DBVR_PacketType(adc_t const *pos);
	adc_t DBVR_EventWordCount(adc_t const *pos);
	adc_t DBVR_ActiveFEBFlags0(adc_t const *pos);
	adc_t DBVR_ActiveFEBFlags1(adc_t const *pos);
	adc_t DBVR_ActiveFEBFlags2(adc_t const *pos);
	adc_t DBVR_TriggerCount(adc_t const *pos);
	adc_t DBVR_EventType(adc_t const *pos);
	adc_t DBVR_ErrorFlags(adc_t const *pos);

	adc_t DBVR_NumHits(adc_t const *pos);

	// CRV DataBlock Payload Accessor Methods (by block address and hit index)
	adc_t DBV_sipmID(adc_t const *pos, size_t hitIdx);
	//  std::array<unsigned int, mu2e::CrvDigi::NSamples> DBV_ADCs(adc_t const *pos, size_t hitIdx);
	std::array<unsigned int, 8> DBV_ADCs(adc_t const *pos, size_t hitIdx);
	adc_t DBV_startTDC(adc_t const *pos, size_t hitIdx);
};

////////////////////////////////////////////////////////////////////////////////
// DataBlock Header Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_ByteCount(adc_t const *pos)
{
	return *(pos + 0);
}

bool mu2e::ArtFragmentReader::DBH_Valid(adc_t const *pos)
{
	return (*(pos + 1) >> 15) & 0x0001;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_ROCID(adc_t const *pos)
{
	return *(pos + 1) & 0x000F;  // 0x000F = 0b1111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_PacketType(adc_t const *pos)
{
	return (*(pos + 1) >> 4) & 0x000F;  // 0x000F = 0b1111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_PacketCount(adc_t const *pos)
{
	return *(pos + 2) & 0x07FF;  // 0x07FF = 0b0111 1111 1111
}

uint64_t mu2e::ArtFragmentReader::DBH_Timestamp(adc_t const *pos)
{
	return uint64_t(*(pos + 3)) + (uint64_t(*(pos + 4)) << 16) + (uint64_t(*(pos + 5)) << 32);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_TimestampLow(adc_t const *pos)
{
	return *(pos + 3);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_TimestampMedium(adc_t const *pos)
{
	return *(pos + 4);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_TimestampHigh(adc_t const *pos)
{
	return *(pos + 5);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_Status(adc_t const *pos)
{
	return *(pos + 6) & 0x00FF;  // 0x00FF = 0b1111 1111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_FormatVersion(adc_t const *pos)
{
	return *(pos + 6) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_EVBMode(adc_t const *pos)
{
	return *(pos + 7) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_SubsystemID(adc_t const *pos)
{
	return (*(pos + 7) >> 6) & 0x0003;  //0x0003 = 0b0011
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_DTCID(adc_t const *pos)
{
	return *(pos + 7) & 0x003F;  // 0x003F = 0b0011 1111
}

////////////////////////////////////////////////////////////////////////////////
// TRK DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////
mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_StrawIndex(adc_t const *pos)
{
	return *(pos + 8 + 0);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TDC0(adc_t const *pos)
{
	return (uint32_t(*(pos + 8 + 1)) & 0xFFFF);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TDC1(adc_t const *pos)
{
	return (uint32_t(*(pos + 8 + 2)) & 0xFFFF);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TOT0(adc_t const *pos)
{
	return (uint32_t(*(pos + 8 + 3)) & 0x00FF);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TOT1(adc_t const *pos)
{
	return ((uint32_t(*(pos + 8 + 3)) >> 8) & 0x00FF);
}

std::array<mu2e::ArtFragmentReader::adc_t, 15> mu2e::ArtFragmentReader::DBT_Waveform(adc_t const *pos)
{
	std::array<adc_t, 15> waveform;

	// Four 12-bit tracker ADC samples fit into every three slots (16 bits * 3)
	// when we pack them tightly

	for (size_t i = 0; i < 4; i += 1) {
		waveform[0 + i * 4] = *(pos + 8 + 4 + i * 3) & 0x0FFF;
		waveform[1 + i * 4] = ((*(pos + 8 + 4 + i * 3 + 1) & 0x00FF) << 4) | (*(pos + 8 + 4 + i * 3) >> 12);
		waveform[2 + i * 4] = ((*(pos + 8 + 4 + i * 3 + 2) & 0x000F) << 8) | (*(pos + 8 + 4 + i * 3 + 1) >> 8);
		if (i < 3) {
			waveform[3 + i * 4] = (*(pos + 8 + 4 + i * 3 + 2) >> 4);
		}
	}

	return waveform;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_Flags(adc_t const *pos)
{
	return (*(pos + 8 + 15) >> 8);
}

////////////////////////////////////////////////////////////////////////////////
// CAL DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBCR_NumHits(adc_t const *pos)
{
	return *(pos + 8 + 0);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_ChannelID(adc_t const *pos, size_t hitIdx)
{
        return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 0) >> 10;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_BoardID(adc_t const *pos, size_t hitIdx)
{
	return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 0) & 0x03FF;
}


mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_Time(adc_t const *pos, size_t hitIdx)
{
	return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 1);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_NumSamples(adc_t const *pos, size_t hitIdx)
{
	return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 2) & 0x00FF;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_PeakSampleIdx(adc_t const *pos, size_t hitIdx)
{
        return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 2) >> 8;
}

std::vector<int> mu2e::ArtFragmentReader::DBC_Waveform(adc_t const *pos, size_t hitIdx)
{
	std::vector<int> waveform(DBC_NumSamples(pos,hitIdx));
	for (size_t i = 0; i < waveform.size(); i++) {
		waveform[i] = *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 3 + i);
	}
	return waveform;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_DIRACOutputA(adc_t const *pos, size_t hitIdx)
{
        return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 3 + DBC_NumSamples(pos,hitIdx) + 0);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_DIRACOutputB(adc_t const *pos, size_t hitIdx)
{
        return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 3 + DBC_NumSamples(pos,hitIdx) + 1);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_ErrorFlags(adc_t const *pos, size_t hitIdx)
{
        return *(pos + 8 + *(pos + 8 + 1 + hitIdx) + 3 + DBC_NumSamples(pos,hitIdx) + 2);
}

////////////////////////////////////////////////////////////////////////////////
// CRV ROC Status Accessor Methods
////////////////////////////////////////////////////////////////////////////////

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_ControllerID(adc_t const *pos)
{
	return *(pos + 8 + 0) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_PacketType(adc_t const *pos)
{
	return *(pos + 8 + 0) & 0x00FF;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_EventWordCount(adc_t const *pos)
{
	return *(pos + 8 + 1);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_ActiveFEBFlags0(adc_t const *pos)
{
	return *(pos + 8 + 3) & 0x00FF;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_ActiveFEBFlags1(adc_t const *pos)
{
	return *(pos + 8 + 3) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_ActiveFEBFlags2(adc_t const *pos)
{
	return *(pos + 8 + 2) & 0x00FF;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_TriggerCount(adc_t const *pos)
{
	return *(pos + 8 + 5);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_EventType(adc_t const *pos)
{
	return *(pos + 8 + 7) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_ErrorFlags(adc_t const *pos)
{
	return *(pos + 8 + 7) & 0x00FF;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBVR_NumHits(adc_t const *pos)
{
	// Currently hardcoded to assume 12 bytes per hit (corresponding to 8 samples)
        adc_t numHits = (*(pos + 8 + 1) - 16) / 12; // Subtract 16 for the ROC header packet

	// Check whether the last hit is actually just empty filler in the last packet
	if(numHits>0) {
	  if(*(pos + 8 + 8 + 6*(numHits-1) + 1) == 0) {
	    numHits = numHits - 1;
	  }
	}

	return numHits;
}

////////////////////////////////////////////////////////////////////////////////
// CRV DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

// Note: The following accessor methods are hardcoded to assume 6 16-bit values
// per CRV hit (corresponding to 8 samples per hit)

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBV_sipmID(adc_t const *pos, size_t hitIdx)
{
	return *(pos + 16 + hitIdx * 6 + 0);
}

//std::array<unsigned int, mu2e::CrvDigi::NSamples> mu2e::ArtFragmentReader::DBV_ADCs(adc_t const *pos, size_t hitIdx) {
//  std::array<unsigned int, mu2e::CrvDigi::NSamples> ADCs;
//  for(size_t i=0; i<mu2e::CrvDigi::NSamples; i+=2) {

// Hardcoding the number of samples per CRV hit to 8
std::array<unsigned int, 8> mu2e::ArtFragmentReader::DBV_ADCs(adc_t const *pos, size_t hitIdx)
{
	std::array<unsigned int, 8> ADCs;
	for (size_t i = 0; i < 8; i += 2) {
		ADCs[i] = *(pos + 16 + hitIdx * 6 + 2 + i / 2) & 0x00FF;
		ADCs[i + 1] = *(pos + 16 + hitIdx * 6 + 2 + i / 2) >> 8;
	}
	return ADCs;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBV_startTDC(adc_t const *pos, size_t hitIdx)
{
	return *(pos + 16 + hitIdx * 6 + 1) & 0x00FF;
}

#endif /* mu2e_artdaq_Overlays_ArtFragmentReader_hh */
