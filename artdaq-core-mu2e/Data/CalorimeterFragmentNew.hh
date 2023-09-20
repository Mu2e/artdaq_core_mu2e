
#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_CalorimeterFragmentNew_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_CalorimeterFragmentNew_HH

#include "artdaq-core-mu2e/Data/ArtFragment.hh"

namespace mu2e {
class CalorimeterFragmentNew : public ArtFragment
{
  public:
    static constexpr int CALORIMETER_FORMAT_VERSION = 1; //TODO
    CalorimeterFragmentNew() : ArtFragment() {}
    CalorimeterFragmentNew(std::vector<uint8_t> data);

    #if HIDE_FROM_ROOT
    explicit CalorimeterFragmentNew(DTCLib::DTC_SubEvent const& f);

    // CalorimeterHitDataPacket: Each hit is readout as a variable length sequence of data packets
    struct CalorimeterHitDataPacket
    {
      uint16_t DetectorType : 3; // subdetector type e.g. CALO=0, CAPHRI = 1, TRAD = 2, LASER = 3
      uint16_t BoardID : 8; //unique board ID from 0 - 255
      uint16_t ChannelNumber : 6; // channel ID from 0-19
      uint16_t DIRACA;
      uint16_t DIRACB;
      uint16_t LastSampleMarkerStart : 12;
      uint16_t LastSampleMarkerEnd : 12;

      // there are 4 types of sample length
      uint16_t SampleType0  : 12;

      uint16_t SampleType1A : 4;
      uint16_t SampleType1B : 8;

      uint16_t SampleType2A : 8;
      uint16_t SampleType2B : 4;

      uint16_t SampleType3A : 10;
      uint16_t SampleType3B : 2;

      uint16_t ErrorFlags;
      uint16_t Time;
      uint8_t NumberOfSamples;
      uint8_t IndexOfMaxDigitizerSample;

      CalorimeterHitDataPacket()
      : DetectorType(0), BoardID(0), ChannelNumber(0), DIRACA(0), DIRACB(0), ErrorFlags(0), Time(0), NumberOfSamples(0), IndexOfMaxDigitizerSample(0) {}
    };
	  
	  
	  // Samples: each hit has a variable number of waveform samples stored contigously using many data packets
	  /*struct CalorimeterSamplePacket
	  {
		  uint16_t SampleType0  : 12;
		  uint16_t SampleType1 : 4;
		  uint16_t SampleType2 : 8;
		  uint16_t SampleType3 : 10;

		  inline uint16_t SampleType0 () const { return Sample0; }
		  inline uint16_t SampleType1 () const { return Sample1; }
		  inline uint16_t SampleType2 () const { return Sample2; }
		  inline uint16_t SampleType3 () const { return Sample3; }

		  inline void SetWaveform(size_t index, uint16_t waveform)
		  {
			  switch (index)
			  {
				  case 0:
					  SampleType0 = waveform & 0x3FF;
					  break;
				  case 1:
					  SampleType1 = waveform & 0x3FF;
					  break;
				  case 2:
					  SampleType2 = waveform & 0x3FF;
					  break;
			    case 3:
					  SampleType3 = waveform & 0x3FF;
					  break;
			  }
			  
		  }
	  };*/
	  
	  // CalorimeterFooterPacket: after transmission of all the hits the event is closed by a final packet containing the informaton about all the chanels that are reconstructed online in the FPGA:
    struct CalorimeterFooterPacket
    {
      uint16_t DetectorType : 3; // subdetector type e.g. CALO=0, CAPHRI = 1, TRAD = 2, LASER = 3
      uint16_t BoardID : 8; //unique board ID from 0 - 255
      uint16_t unused : 1;
      uint16_t ChannelStatusFlagA : 4;
      uint16_t ChannelStatusFlagC;
      uint16_t DIRACA;
      uint16_t DIRACB;
      uint16_t DIRACC;
      uint16_t DIRACD;
      uint16_t DIRACE;
      uint16_t DIRACF;
      CalorimeterFooterPacket ()
      : DetectorType(0), BoardID(0), unused(0), ChannelStatusFlagA(0), ChannelStatusFlagC(0) {}

    };

    std::vector<std::pair<CalorimeterHitDataPacket, std::vector<uint16_t>>> GetCalorimeterHitData(size_t blockIndex) const;
    std::unique_ptr<CalorimeterFooterPacket> GetCalorimeterFooter(size_t blockIndex) const;
    std::vector<std::pair<CalorimeterHitDataPacket, uint16_t>> GetCalorimeterHitsForTrigger(size_t blockIndex) const;
    #endif
  };
}  // namespace mu2e

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_CalorimeterFragmentNew_HH
