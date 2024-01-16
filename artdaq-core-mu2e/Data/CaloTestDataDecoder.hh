#ifndef ARTDAQ_CORE_MU2E_DATA_CaloTestDataDecoder_HH
#define ARTDAQ_CORE_MU2E_DATA_CaloTestDataDecoder_HH

#include "artdaq-core-mu2e/Data/DTCDataDecoder.hh"


namespace mu2e {
class CaloTestDataDecoder : public DTCDataDecoder
{
  public:

	CaloTestDataDecoder() : DTCDataDecoder() {}

	CaloTestDataDecoder(std::vector<uint8_t> data);

    explicit CaloTestDataDecoder(DTCLib::DTC_SubEvent const& f);

    // CaloTestHitDataPacket: Each hit is readout as a variable length sequence of data packets
    struct CaloTestHitDataPacket
    {
      uint16_t DetectorType : 3;
      CaloTestHitDataPacket()
      : DetectorType(0) {}
    };
	  
	  
    struct CaloTestFooterPacket
    {
      uint16_t DetectorType : 3; 
      
      CaloTestFooterPacket ()
      : DetectorType(0) {}

    };

    std::vector<std::pair<CaloTestHitDataPacket, std::vector<uint16_t>>>* GetCalorimeterHitData(size_t blockIndex) const;
    std::unique_ptr<CaloTestFooterPacket> GetCalorimeterFooter(size_t blockIndex) const;
    std::vector<std::pair<CaloTestHitDataPacket, uint16_t>> GetCalorimeterHitsForTrigger(size_t blockIndex) const;
    
  };
}  // namespace mu2e
#endif 
