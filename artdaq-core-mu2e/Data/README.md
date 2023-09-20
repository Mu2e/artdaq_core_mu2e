# Documentation: Calorimeter Fragments

## Header File

* The CalorimeterFragments header should contain packet details as descirbed in Mu2e-doc-db 4914.

* The CalorimeterFragment object inherits from ArtFragment.hh

* In that document the following are defined:

### CalorimeterHitDataPacket

* This is a per hit packet and is a variable length sequence of data packets (see page 13)

### CalorimeterFooterPacket

* After transmission of all the hits the event is closed by a final packet continaing information about all the channels that are reconstructed online in the FPGA.

## The Source File

### CalorimeterFragmentNew(DTCLib::DTC_SubEvent const& evt)

* This is a check on the DTC header that the system and version is correct.

### CalorimeterFragmentNew(std::vector<uint8_t> data)

* This is a check on the DTC header that the system and version is correct.

### std::vector<std::pair<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket, std::vector<uint16_t>>> mu2e::CalorimeterFragmentNew::GetCalorimeterHitData(size_t blockIndex) const

* Accesses data, makes a list called output, recasts the DataPackets
* Loops over waveforms and fill the output pair with the samples

### std::vector<std::pair<mu2e::CalorimeterFragmentNew::CalorimeterHitDataPacket, uint16_t>> mu2e::CalorimeterFragmentNew::GetCalorimeterHitsForTrigger(size_t blockIndex) const

* Similar to previous but for quick trigger purposes

## ArtBinaryPacketsFromDigis

This module is updated including:

* CaloDataPacket updated to remove BoardID

The following are functions related to the calorimeter:

```
  //--------------------------------------------------------------------------------
  //  methods used to handle the calorimeter data
  //--------------------------------------------------------------------------------
  void   fillCalorimeterDataPacket(CaloDAQMap const& calodaqconds,
                                   const CaloDigi& SD, CaloDataPacket& caloData);

  void addCaloHitToCaloPacket(calo_data_block_t& dataBlock, CaloDataPacket& caloData);

  void   fillCalorimeterHeaderDataPacket(CaloDAQMap const& calodaqconds,
                                         const CaloDigi& SD, DataBlockHeader& HeaderData, uint64_t& EventNum);

  void fillHeaderByteAndPacketCounts(calo_data_block_t& caloData);

  void processCalorimeterData(art::Event& evt, uint64_t& eventNum,
                              calo_data_block_list_t& caloDataBlocks);

  void fillCalorimeterDMABlocks(DTCLib::DTC_Event& currentEvent, calo_data_block_list_t& caloData);

  void fillCalorimeterDataStream(DTCLib::DTC_Event& currentEvent, calo_data_block_t& caloData);

  void printCalorimeterData(CaloDataPacket const& curDataBlock);

  const size_t waveformMaximumIndex(std::vector<adc_t> const& waveform);

```
where:

* fillCalorimeterDataPacket

This function extracts the Raw ID (CaloSiPMId) information, files the calorimeterHitData packet with relevent details


* addCaloHitToCaloPacket

This function adds a new hit to the vector of hits which already exists.


* fillCalorimeterHeaderDataPacket

This function also looks at the Raw ID and extracts useful information for the HeaderDataPacket

* fillHeaderByteAndPacketCounts

This function adds packet counts to the header number of bytes

* processCalorimeterData

This function loops over the digi collection. It extracts a given CaloDigi.

The code then loops over the collection and calls:  fillCalorimeterHeaderDataPacket and fillCalorimeterDataPacket passing in the calo digi to each.

There is then a loop over ROC and DTC and finds all hits for this event coming from the specified DTC/ROC combination.


* fillHeaderByteAndPacketCounts

fills the byte count

* fillCalorimeterDMABlocks

Calls fillCalorimeterDataStream, adds the current DataBlock to the current SuperBlock

* fillCalorimeterDataStream

* printCalorimeterData

