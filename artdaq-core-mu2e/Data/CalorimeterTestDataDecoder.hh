#ifndef ARTDAQ_CORE_MU2E_DATA_CALORIMETERTESTDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CALORIMETERTESTDATADECODER_HH

#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"

#include <messagefacility/MessageLogger/MessageLogger.h> // Putting this here so that Offline/DAQ/src/FragmentAna_module.cc can use it

namespace mu2e {
  class CalorimeterTestDataDecoder : public DTCDataDecoder
  {
  public:

    CalorimeterTestDataDecoder() : DTCDataDecoder() {}

    CalorimeterTestDataDecoder(std::vector<uint8_t> data);

    CalorimeterTestDataDecoder(DTCLib::DTC_SubEvent const& f);

    // CalorimeterHitTestDataPacket: Each hit is readout as a variable length sequence of data packets
    struct CalorimeterHitTestDataPacket
    {
      uint16_t BeginMarker : 12; //0xAAA
      uint16_t BoardID : 12; //unique board ID from 0 - 255
      uint16_t ChannelID : 12; // channel ID from 0-19
      uint16_t InPayloadEventWindowTag : 12;
 
      // there are 4 types of sample length
      uint16_t SampleType0  : 12;

      uint16_t SampleType1A : 4;
      uint16_t SampleType1B : 8;

      uint16_t SampleType2A : 8;
      uint16_t SampleType2B : 4;

      uint16_t LastSampleMarker : 12;
      uint16_t ErrorFlags : 12;
      uint32_t Time : 24;
      uint16_t IndexOfMaxDigitizerSample : 12;
      uint16_t NumberOfSamples : 12;

      CalorimeterHitTestDataPacket()
	: BoardID(0), ChannelID(0), InPayloadEventWindowTag(0), ErrorFlags(0), Time(0), IndexOfMaxDigitizerSample(0), NumberOfSamples(0) {}
    };
	  
	  
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

    std::vector<std::pair<CalorimeterHitTestDataPacket, std::vector<uint16_t>>>* GetCalorimeterHitData(size_t blockIndex) const;
    std::unique_ptr<CalorimeterFooterPacket> GetCalorimeterFooter(size_t blockIndex) const;
    std::vector<std::pair<CalorimeterHitTestDataPacket, uint16_t>> GetCalorimeterHitsForTrigger(size_t blockIndex) const;
    
  };
}  // namespace mu2e
#endif 
