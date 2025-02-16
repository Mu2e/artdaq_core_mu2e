#include "artdaq-core-mu2e/Data/CalorimeterDataDecoder.hh"

#include "TRACE/tracemf.h"

#include <algorithm>
#include <cmath>


namespace mu2e {

  CalorimeterDataDecoder::CalorimeterDataDecoder(DTCLib::DTC_SubEvent const& evt)
    : DTCDataDecoder(evt)
  {
    if (block_count() > 0){
      auto dataPtr = dataAtBlockIndex(0);// Return pointer to beginning of DataBlock at given DataBlock index( returns type DTCLib::DTC_DataBlock)
      auto hdr = dataPtr->GetHeader(); // get the header
      // check that the subsystem is the calo and the version is correct:
      if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter || hdr->GetVersion() > 1){
        //TLOG(TLVL_WARNING) << "CalorimeterDataDecoder CONSTRUCTOR: First block has unexpected type/version " << static_cast<int>(hdr->GetSubsystem()) << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Calorimeter) << "/[0,1])";
      }
    }
  }

  CalorimeterDataDecoder::CalorimeterDataDecoder(std::vector<uint8_t> data)
    : DTCDataDecoder(data)
  {
    //event_.GetDataBlockCount() > 0
    if (block_count() > 0){
      auto dataPtr = dataAtBlockIndex(0);
      auto hdr = dataPtr->GetHeader();
      if (hdr->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter || hdr->GetVersion() > 1){
        //TLOG(TLVL_WARNING) << "CalorimeterDataDecoder CONSTRUCTOR: First block has unexpected type/version " << hdr->GetSubsystem() << "/" << static_cast<int>(hdr->GetVersion()) << " (expected " << static_cast<int>(DTCLib::DTC_Subsystem_Calorimeter) << "/[0,1])";
      }
    }
  }


  // Get Calo Hit Data Packet
  std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, std::vector<uint16_t>>>* mu2e::CalorimeterDataDecoder::GetCalorimeterHitData(size_t blockIndex) const
  {
    // a pair is created mapping the data packet to set of hits (?)
    std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, std::vector<uint16_t>>> *output = new std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, std::vector<uint16_t>>>();
    
    // get data block at given index
    auto dataPtr = dataAtBlockIndex(blockIndex);
    if (dataPtr == nullptr) return output;
  
    // check size of hit data packet
    static_assert(sizeof(mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket) % 2 == 0);
    
    auto dataPacket = reinterpret_cast<CalorimeterHitDataPacket const*>(dataPtr->GetData());
    
    // pos is a uint16_t pointer after the BoardID in the data stream
    uint16_t const* pos = 0;//assumes this is the first piece of information
  
    // loop over samples:
    unsigned int count = 0;
    while(count < dataPacket->NumberOfSamples){
      
      // Reinterpret pos as a pointer to a hit readout header
        output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket(*reinterpret_cast<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket const*>(pos)), std::vector<uint16_t>()); //Construct and insert element at the end, Converts between types by reinterpreting the underlying bit pattern. 
      
      // Step pos past the hit readout
      pos += sizeof(mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket) / sizeof(uint16_t);
  
      // Setup waveform storage
      // find number of samples from output
      auto nSamples = output->back().first.NumberOfSamples;
      // resize the vector part to nSamples
      output->back().second.resize(nSamples);
  
      // Copy waveform into output
      memcpy(output->back().second.data(), pos, sizeof(uint16_t) * nSamples);
  
      // Step pos past waveform
      pos += nSamples;
      count++;
    }
    return output;
  }


  // Get Calo Hit Data Packet (Paolo)
  /*
  std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, std::vector<uint16_t>>>* mu2e::CalorimeterDataDecoder::GetCalorimeterHitData(size_t blockIndex) const
  {
    std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, std::vector<uint16_t>>> *output = new std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, std::vector<uint16_t>>>();
  
    // get data block at given index
    DTCLib::DTC_DataBlock const * dataBlock = dataAtBlockIndex(blockIndex);
    if (dataBlock == nullptr) return output;

    DTCLib::DTC_DataHeaderPacket* blockHeader = dataBlock->GetHeader().get();
    size_t blockSize = dataBlock->byteSize;
    size_t nPackets = blockHeader->GetPacketCount();
    size_t dataSize = blockSize - 16;

    auto blockDataPtr = dataBlock->GetData();

    if (nPackets == 0){ //Empty packet
      TLOG(TLVL_DEBUG) << "CalorimeterDataDecoder::GetCalorimeterHitData : no packets -- disabled ROC?\n";
      return output;
    }
    
    auto blockPos = reinterpret_cast<const uint8_t*>(blockDataPtr); //byte position in block (multiple of 16)
    while(blockPos < reinterpret_cast<const uint8_t*>(blockDataPtr) + dataSize){ //until the end of this block      

      //Create output 
      output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket(), std::vector<uint16_t>());

      //Before waveform (48 bit)
      auto hitPtr = reinterpret_cast<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket const*>(blockPos);
      output->back().first.DetectorType = hitPtr->DetectorType;
      output->back().first.BoardID = hitPtr->BoardID;
      output->back().first.ChannelNumber = hitPtr->ChannelNumber;
      output->back().first.DIRACA = hitPtr->DIRACA;
      output->back().first.DIRACB = hitPtr->DIRACB;

      //Waveform
      auto waveformPtr = reinterpret_cast<mu2e::CalorimeterDataDecoder::Calorimeter12bitWord const*>(blockPos + 6);
           
      //Search for 0xFFF
      uint nWordsMax = (((dataSize - 6) * 8) / 12);
      int lastSampleMarkerIndex = -1;
      for (uint i=0; i<nWordsMax; i++){
        if (waveformPtr[i].word == 0xFFF){
          lastSampleMarkerIndex = i;
          break;
        }
      }
      if (lastSampleMarkerIndex == -1){
        TLOG(TLVL_ERROR) << "CalorimeterDataDecoder::GetCalorimeterHitData : LastSampleMarker 0xFFF not found in the payload!" << std::endl;
        return output;
      }

      //Save waveform
      size_t nSamples = lastSampleMarkerIndex;
      output->back().second.resize(nSamples);
      for (uint i=0; i<nSamples; i++){
        output->back().second[i] = waveformPtr[i].word;
      }

      //After waveform
      output->back().first.LastSampleMarker = waveformPtr[lastSampleMarkerIndex].word;
      output->back().first.ErrorFlags = waveformPtr[lastSampleMarkerIndex+1].word;
      output->back().first.Time = (waveformPtr[lastSampleMarkerIndex+2].word << 12) | waveformPtr[lastSampleMarkerIndex+3].word ;
      output->back().first.IndexOfMaxDigitizerSample = waveformPtr[lastSampleMarkerIndex+4].word;
      output->back().first.NumberOfSamples = waveformPtr[lastSampleMarkerIndex+5].word;

      //Advance to the next 16-byte packet
      float hitByteSize = nSamples*1.5 + sizeof(output->back().first);
      uint8_t hitPackets = uint8_t(std::ceil(hitByteSize/16)); //number of 16-byte packets this hit occupied
      blockPos += hitPackets*16; //advance by 16 bytes per packet
    }

    return output;
  }
  */

  std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket, uint16_t>> mu2e::CalorimeterDataDecoder::GetCalorimeterHitsForTrigger(size_t blockIndex) const
  {
    std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket,uint16_t>> output;
  
    auto dataPtr = dataAtBlockIndex(blockIndex);
    if (dataPtr == nullptr) return output;
  
    static_assert(sizeof(CalorimeterHitDataPacket) % 2 == 0);
  
    auto dataPacket = reinterpret_cast<mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket const*>(dataPtr->GetData());
    
    uint16_t const* pos = 0;
  
    // loop over samples:
    unsigned int count = 0;
    while(count < dataPacket->NumberOfSamples){
      // Reinterpret pos as a pointer to a hit readout header
      output.emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket(*reinterpret_cast<CalorimeterHitDataPacket const*>(pos)), 0);
      // Step pos past the hit readout
      pos += sizeof(mu2e::CalorimeterDataDecoder::CalorimeterHitDataPacket) / sizeof(uint16_t);
  
      output.back().second = *(pos + output.back().first.IndexOfMaxDigitizerSample);
  
      // Step pos past waveform
      auto nSamples = output.back().first.NumberOfSamples;
      pos += nSamples;
    }
    return output;
  }


  // Get Calo Hit Test Data Packet
  std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitTestDataPacket, std::vector<uint16_t>>>* mu2e::CalorimeterDataDecoder::GetCalorimeterHitTestData(size_t blockIndex) const
  {
    std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitTestDataPacket, std::vector<uint16_t>>> *output = new std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterHitTestDataPacket, std::vector<uint16_t>>>();
  
    // get data block at given index
    DTCLib::DTC_DataBlock const * dataBlock = dataAtBlockIndex(blockIndex);
    if (dataBlock == nullptr){ //Empty block
      TLOG(TLVL_WARNING) << "CalorimeterDataDecoder::GetCalorimeterHitTestData : empty block " << blockIndex;
      return output;
    }

    if(dataBlock->GetHeader()->GetSubsystem() != DTCLib::DTC_Subsystem_Calorimeter) {
      TLOG(TLVL_DEBUG) << "CalorimeterDataDecoder::GetCalorimeterHitTestData : this block is from different subsystem: " << dataBlock->GetHeader()->GetSubsystem();
      return output;
    }

    DTCLib::DTC_DataHeaderPacket* blockHeader = dataBlock->GetHeader().get();
    size_t blockSize = dataBlock->byteSize;
    size_t nPackets = blockHeader->GetPacketCount();
    size_t dataSize = blockSize - 16;

    auto blockDataPtr = dataBlock->GetData();

    if (nPackets == 0){ //Empty packet
      TLOG(TLVL_DEBUG) << "CalorimeterDataDecoder::GetCalorimeterHitTestData : no packets in block " << blockIndex << " -- disabled ROC?\n";
      return output;
    }
    
    auto blockPos = reinterpret_cast<const uint8_t*>(blockDataPtr); //byte position in block (multiple of 16)
    while(blockPos < reinterpret_cast<const uint8_t*>(blockDataPtr) + dataSize){ //until the end of this block      

      mu2e::CalorimeterDataDecoder::Data12bitReader reader(reinterpret_cast<const uint16_t*>(blockPos));
      
      //Make sure first word is 0xAAA
      if (reader[0] != 0xAAA){
        TLOG(TLVL_ERROR) << "CalorimeterDataDecoder::GetCalorimeterHitTestData : in block " << blockIndex << " hit " << output->size() << " BeginMarker is " << std::hex << reader[0] << std::dec << " instead of 0xAAA\n";
        //Return minimal hit and stop decoding this ROC
        output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterHitTestDataPacket(), std::vector<uint16_t>());
        output->back().first.BeginMarker = reader[0];
        return output;
      }
      
      //Search for 0xFFF
      uint nWordsMax = ((dataSize * 8) / 12);
      int lastSampleMarkerIndex = -1;
      for (uint i=4; i<nWordsMax; i++){ //waveform starts from 5th word
        if (reader[i] == 0xFFF){
          lastSampleMarkerIndex = i;
          break;
        }
      }

      //0xFFF not found
      if (lastSampleMarkerIndex == -1){
        TLOG(TLVL_ERROR) << "CalorimeterDataDecoder::GetCalorimeterHitTestData : LastSampleMarker 0xFFF not found in the payload!" << std::endl;
        //Return minimal hit and stop decoding this ROC
        output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterHitTestDataPacket(), std::vector<uint16_t>());
        output->back().first.LastSampleMarker = 0;
        return output;
      }

      //Create output
      output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterHitTestDataPacket(), std::vector<uint16_t>());

      //Before waveform
      output->back().first.BeginMarker = reader[0];
      output->back().first.BoardID = reader[1];
      output->back().first.ChannelID = reader[2];
      output->back().first.InPayloadEventWindowTag = reader[3];

      //waveform
      size_t nSamples = lastSampleMarkerIndex-4;
      output->back().second.resize(nSamples);
      for (uint i=0; i<nSamples; i++){
        output->back().second[i] = reader[4+i];
      }

      //After waveform
      output->back().first.LastSampleMarker = reader[lastSampleMarkerIndex];
      output->back().first.ErrorFlags = reader[lastSampleMarkerIndex+1];
      output->back().first.Time = reader[lastSampleMarkerIndex+2] | (reader[lastSampleMarkerIndex+3] << 12) ;
      output->back().first.IndexOfMaxDigitizerSample = reader[lastSampleMarkerIndex+4];
      output->back().first.NumberOfSamples = reader[lastSampleMarkerIndex+5];    

      //Waveform reading check
      if (output->back().first.NumberOfSamples != nSamples){
        TLOG(TLVL_ERROR) << "CalorimeterDataDecoder::GetCalorimeterHitTestData : "
                         << "in block " << blockIndex << " hit " << output->size()
                         << " NumberOfSamples is " << output->back().first.NumberOfSamples
                         << " but waveform is " << nSamples << " samples long\n";
        return output;
      }

      //Advance to the next 16-byte packet
      size_t totalWordsRead = lastSampleMarkerIndex+6;
      size_t nTwoPacketsRead = totalWordsRead/21; //There are 21 12-bit words every 2 packets
      float nBytesRead = totalWordsRead*1.5 + (0.5*nTwoPacketsRead); //12 bits for every word + 4 extra bits every 2 packets
      uint8_t hitPackets = uint8_t(std::ceil(nBytesRead/16)); //number of 16-byte packets this hit occupied
      blockPos += hitPackets*16; //advance by 16 bytes per packet
    }

    return output;
  }


  // Get Calo Counters Data Packet
  std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket, std::vector<uint32_t>>>* mu2e::CalorimeterDataDecoder::GetCalorimeterCountersData(size_t blockIndex) const
  {
    std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket, std::vector<uint32_t>>> *output = new std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket, std::vector<uint32_t>>>();
  
    // get data block at given index
    DTCLib::DTC_DataBlock const * dataBlock = dataAtBlockIndex(blockIndex);
    if (dataBlock == nullptr) return output;

    DTCLib::DTC_DataHeaderPacket* blockHeader = dataBlock->GetHeader().get();
    size_t blockSize = dataBlock->byteSize;
    size_t nPackets = blockHeader->GetPacketCount();
    size_t dataSize = blockSize - 16;

    auto blockDataPtr = dataBlock->GetData();

    if (nPackets == 0){ //Empty packet
      TLOG(TLVL_DEBUG) << "CalorimeterDataDecoder::GetCalorimeterCountersData : no packets -- disabled ROC?";
      return output;
    }

    if (dataSize%4 != 0){ //Data not multiple of 32-bit words
      TLOG(TLVL_WARNING) << "CalorimeterDataDecoder::GetCalorimeterCountersData : data size (" << dataSize << ") is not multiple of 32-bit";
      return output;
    }
    
    uint16_t nCounters = dataSize / 4;
    //Create output 
    output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket(), std::vector<uint32_t>(nCounters));
    output->back().first.numberOfCounters = nCounters;

    //Get data in 32-bit words
    auto data32bitPtr = reinterpret_cast<const uint32_t *>(blockDataPtr);
    for (uint word=0; word<nCounters; word++){
      output->back().second[word] = (*(data32bitPtr + word));
    }

    return output;
  }


  // Get EmulatedROC Counters Data Packet
  std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket, std::vector<uint32_t>>>* mu2e::CalorimeterDataDecoder::GetEmulatedCountersData(size_t blockIndex) const
  {
    std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket, std::vector<uint32_t>>> *output = new std::vector<std::pair<mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket, std::vector<uint32_t>>>();
  
    // get data block at given index
    DTCLib::DTC_DataBlock const * dataBlock = dataAtBlockIndex(blockIndex);
    if (dataBlock == nullptr) return output;

    DTCLib::DTC_DataHeaderPacket* blockHeader = dataBlock->GetHeader().get();
    size_t blockSize = dataBlock->byteSize;
    size_t nPackets = blockHeader->GetPacketCount();
    size_t dataSize = blockSize - 16;

    auto blockDataPtr = dataBlock->GetData();

    if (nPackets == 0){ //Empty packet
      TLOG(TLVL_DEBUG) << "CalorimeterDataDecoder::GetEmulatedCountersData : no packets -- disabled ROC?";
      return output;
    }

    if (dataSize%4 != 0){ //Data not multiple of 32-bit words
      TLOG(TLVL_WARNING) << "CalorimeterDataDecoder::GetEmulatedCountersData : data size (" << dataSize << ") is not multiple of 32-bit";
      return output;
    }
    
    uint16_t nCounters = dataSize / 4;
    //Create output 
    output->emplace_back(mu2e::CalorimeterDataDecoder::CalorimeterCountersDataPacket(), std::vector<uint32_t>(nCounters));
    output->back().first.numberOfCounters = nCounters;

    //Get data in 32-bit words
    auto data32bitPtr = reinterpret_cast<const uint32_t *>(blockDataPtr);
    for (uint word=0; word<nCounters; word++){
      output->back().second[word] = (*(data32bitPtr + word));
    }

    return output;
  }

} // namespace mu2e
