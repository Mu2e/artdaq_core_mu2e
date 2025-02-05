#ifndef ARTDAQ_CORE_MU2E_DATA_CALORIMETERDATADECODER_HH
#define ARTDAQ_CORE_MU2E_DATA_CALORIMETERDATADECODER_HH


#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"

#include <messagefacility/MessageLogger/MessageLogger.h> // Putting this here so that Offline/DAQ/src/FragmentAna_module.cc can use it

namespace mu2e {
  class CalorimeterDataDecoder : public DTCDataDecoder
  {
  public:

    CalorimeterDataDecoder() : DTCDataDecoder() {}

    CalorimeterDataDecoder(std::vector<uint8_t> data);

    CalorimeterDataDecoder(DTCLib::DTC_SubEvent const& f);

    //Class to swap pairs of 16-bit words and extract 12-bit words without memory buffers -- only applies to DEBUG data
    class Data12bitReader {
      private:
        const uint16_t* dataPtr;

      public:
        Data12bitReader(const uint16_t* dataPtr) : dataPtr(dataPtr) {}

        uint16_t operator[](size_t index) const { //FIXME: This algorithm is very complicated, might want to optimize

          int wordInTwoPackets = index % 21;
          int nTwoPackets = (index - wordInTwoPackets) / 21;
          int startingLetter = wordInTwoPackets % 4;

          size_t wordIndex1 = nTwoPackets*16 + ((wordInTwoPackets * 3) / 4);
          size_t wordIndex2 = wordIndex1+1;
          uint16_t word1 = dataPtr[wordIndex1%2==0?wordIndex1+1:wordIndex1-1];
          uint16_t word2 = dataPtr[wordIndex2%2==0?wordIndex2+1:wordIndex2-1];

          uint16_t temp;
          switch (startingLetter) {
            case 0:
              temp = (word1 >> 4) & 0x0FFF;
              break;
            case 1:
              temp = ((word1 & 0x000F) << 8) | ((word2 & 0xFF00) >> 8);
              break;
            case 2:
              temp = ((word1 & 0x00FF) << 4) | ((word2 & 0xF000) >> 12);
              break;
            case 3:
              temp = word1 & 0x0FFF;
              break;
          }

          return temp;
        }
    };

    // CalorimeterHitDataPacket: Each hit is readout as a variable length sequence of data packets
    struct CalorimeterHitDataPacket
    {
      uint16_t DetectorType : 3; // subdetector type e.g. CALO=0, CAPHRI = 1, TRAD = 2, LASER = 3
      uint16_t BoardID : 8; //unique board ID from 0 - 255
      uint16_t ChannelNumber : 5; // channel ID from 0-19
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

    struct CalorimeterHitTestDataPacket
    {
      uint16_t BeginMarker : 12; //0xAAA
      uint16_t BoardID : 12; //unique board ID from 0 - 255
      uint16_t ChannelID : 12; // channel ID from 0-19
      uint16_t InPayloadEventWindowTag : 12;
       
      uint16_t LastSampleMarker : 12; //0xFFF
      uint16_t ErrorFlags : 12;
      uint32_t Time : 24;
      uint16_t IndexOfMaxDigitizerSample : 12;
      uint16_t NumberOfSamples : 12;

      CalorimeterHitTestDataPacket()
      : BeginMarker(0), BoardID(0), ChannelID(0), InPayloadEventWindowTag(0), LastSampleMarker(0), ErrorFlags(0), Time(0), IndexOfMaxDigitizerSample(0), NumberOfSamples(0) {}
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

    struct Calorimeter12bitWord
    {
      uint16_t word : 12;
      Calorimeter12bitWord() : word(0) {}
    };

    struct CalorimeterCountersDataPacket
    {
      uint16_t numberOfCounters;
      CalorimeterCountersDataPacket() : numberOfCounters(0) {}
    };

    std::vector<std::pair<CalorimeterHitDataPacket, std::vector<uint16_t>>>* GetCalorimeterHitData(size_t blockIndex) const;
    std::vector<std::pair<CalorimeterHitTestDataPacket, std::vector<uint16_t>>>* GetCalorimeterHitTestData(size_t blockIndex) const;
    std::vector<std::pair<CalorimeterCountersDataPacket, std::vector<uint32_t>>>* GetCalorimeterCountersData(size_t blockIndex) const;
    std::vector<std::pair<CalorimeterCountersDataPacket, std::vector<uint32_t>>>* GetEmulatedCountersData(size_t blockIndex) const;
    std::unique_ptr<CalorimeterFooterPacket> GetCalorimeterFooter(size_t blockIndex) const;
    std::vector<std::pair<CalorimeterHitDataPacket, uint16_t>> GetCalorimeterHitsForTrigger(size_t blockIndex) const;
  };

  using CalorimeterDataDecoders = std::vector<CalorimeterDataDecoder>;
}  // namespace mu2e
#endif /* mu2e_artdaq_Data_CalorimeterDataDecoder_hh */
