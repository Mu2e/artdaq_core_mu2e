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

#include <iostream>

namespace mu2e {
  class ArtFragmentReader;
}

class mu2e::ArtFragmentReader: public mu2e::ArtFragment {
  public:

  ArtFragmentReader(artdaq::Fragment const & f) : ArtFragment(f) {};

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
  std::array<adc_t,15> DBT_Waveform(adc_t const *pos);
  adc_t DBT_Flags(adc_t const *pos);

  // CAL DataBlock Payload Accessor Methods (by block address)
  adc_t DBC_CrystalID(adc_t const *pos);
  adc_t DBC_apdID(adc_t const *pos);
  adc_t DBC_Time(adc_t const *pos);
  adc_t DBC_NumSamples(adc_t const *pos);
  adc_t DBC_PeakSampleIdx(adc_t const *pos);
  std::vector<int> DBC_Waveform(adc_t const *pos);

};

////////////////////////////////////////////////////////////////////////////////
// DataBlock Header Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_ByteCount(adc_t const *pos) {
  return *(pos+0);
}

bool mu2e::ArtFragmentReader::DBH_Valid(adc_t const *pos) {
  return (*(pos+1) >> 15) & 0x0001;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_ROCID(adc_t const *pos) {
  return *(pos+1) & 0x000F; // 0x000F = 0b1111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_RingID(adc_t const *pos) {
  return (*(pos+1) >> 8) & 0x0007; // 0x0007 = 0b0111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_PacketType(adc_t const *pos) {
  return (*(pos+1) >> 4) & 0x000F; // 0x000F = 0b1111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_PacketCount(adc_t const *pos) {
  return *(pos+2) & 0x07FF; // 0x07FF = 0b0111 1111 1111
}

uint64_t mu2e::ArtFragmentReader::DBH_Timestamp(adc_t const *pos) {
  return uint64_t(*(pos+3)) + (uint64_t(*(pos+4)) << 16) + (uint64_t(*(pos+5)) << 32);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_TimestampLow(adc_t const *pos) {
  return *(pos+3);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_TimestampMedium(adc_t const *pos) {
  return *(pos+4);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_TimestampHigh(adc_t const *pos) {
  return *(pos+5);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_Status(adc_t const *pos) {
  return *(pos+6) & 0x00FF; // 0x00FF = 0b1111 1111
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_FormatVersion(adc_t const *pos) {
  return *(pos+6) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_EVBMode(adc_t const *pos) {
  return *(pos+7) >> 8;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_SubsystemID(adc_t const *pos) {
  return (*(pos+7) >> 6) & 0x0003; //0x0003 = 0b0011
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBH_DTCID(adc_t const *pos) {
  return *(pos+7) & 0x003F; // 0x003F = 0b0011 1111
}

////////////////////////////////////////////////////////////////////////////////
// TRK DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////
mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_StrawIndex(adc_t const *pos) {
  return *(pos+8+0);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TDC0(adc_t const *pos) {
  return (uint32_t(*(pos+8+1)) & 0xFFFF);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TDC1(adc_t const *pos) {
  return (uint32_t(*(pos+8+2)) & 0xFFFF);
}


mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TOT0(adc_t const *pos) {
  return (uint32_t(*(pos+8+3)) & 0x00FF) ;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_TOT1(adc_t const *pos) {
  return ( (uint32_t(*(pos+8+3)) >> 8) & 0x00FF) ;
}

std::array<mu2e::ArtFragmentReader::adc_t,15> mu2e::ArtFragmentReader::DBT_Waveform(adc_t const *pos) {
  std::array<adc_t,15> waveform;

  // Four 12-bit tracker ADC samples fit into every three slots (16 bits * 3)
  // when we pack them tightly

  for (size_t i = 0; i < 4; i+=1){
    waveform[0 + i*4]   = *(pos+8+4+i*3) & 0x0FFF				       ;
    waveform[1 + i*4]   = ((*(pos+8+4+i*3+1) & 0x00FF) << 4) | (*(pos+8+4+i*3) >> 12)  ;
    waveform[2 + i*4]   = ((*(pos+8+4+i*3+2) & 0x000F) << 8) | (*(pos+8+4+i*3+1) >> 8) ;
    if(i<3) {
      waveform[3 + i*4] = (*(pos+8+4+i*3+2) >> 4)                                      ;
    }
  }

  return waveform;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBT_Flags(adc_t const *pos) {
  
  return (*(pos+8+15) >> 8);
}


////////////////////////////////////////////////////////////////////////////////
// CAL DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_CrystalID(adc_t const *pos) {
  return *(pos+8+0) & 0x0FFF; // 0x0FFF = 0b1111 1111 1111   
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_apdID(adc_t const *pos) {
  return *(pos+8+0) >> 12;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_Time(adc_t const *pos) {
  return *(pos+8+1);
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_NumSamples(adc_t const *pos) {
  return *(pos+8+2) & 0x00FF;
}

mu2e::ArtFragmentReader::adc_t mu2e::ArtFragmentReader::DBC_PeakSampleIdx(adc_t const *pos) {
  return *(pos+8+2) >> 8;
}

std::vector<int> mu2e::ArtFragmentReader::DBC_Waveform(adc_t const *pos) {
  std::vector<int>  waveform;
  for(size_t i=0; i<DBC_NumSamples(pos); i++) {
    waveform.push_back(*(pos+8+3+i));
  }
  return waveform;
}





#endif /* mu2e_artdaq_Overlays_ArtFragmentReader_hh */

