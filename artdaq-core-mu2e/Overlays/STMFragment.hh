#ifndef MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
#define MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH

#include "artdaq-core/Data/Fragment.hh"

namespace mu2e {
class STMFragment
{
public:
	explicit STMFragment(artdaq::Fragment const& f)
		: artdaq_fragment_(f) {}

	struct STMDataPacket
	{  // Remeber testbeam data is little endian
		// uint16_t FixedHeaderWord[16];
		std::vector<uint16_t> data;
		STMDataPacket() {}
	};

	/*
		struct STMBoardID
		{
			uint16_t BoardID : 10;
			uint16_t ChannelStatusFlagsA : 6;
			uint16_t ChannelStatusFlagsB : 14;
			uint16_t unused : 2;

			STMBoardID()
				: BoardID(0), ChannelStatusFlagsA(0), ChannelStatusFlagsB(0), unused(0) {}
		};

		struct STMHitReadoutPacket
		{
			uint16_t ChannelNumber : 6;
			uint16_t DIRACA : 10;
			uint16_t DIRACB;
			uint16_t ErrorFlags;
			uint16_t Time;
			uint8_t NumberOfSamples;
			uint8_t IndexOfMaxDigitizerSample;

			STMHitReadoutPacket()
				: ChannelNumber(0), DIRACA(0), DIRACB(0), ErrorFlags(0), Time(0), NumberOfSamples(0), IndexOfMaxDigitizerSample(0) {}
		};
	*/

	class STM_tHdr
	{
	public:
		uint16_t sw_tHdr[20];
		STM_tHdr(){};

		// Reverse order - data is little endian.
		uint32_t testWord() const
		{
			uint32_t tmp = (sw_tHdr[1] << 16) | sw_tHdr[0];
			return tmp;
		};
		uint32_t dataSize() const
		{
			uint32_t tmp = (sw_tHdr[3] << 16) | sw_tHdr[2];
			return tmp;
		};
		uint32_t sliceNumber() const
		{
			uint32_t tmp = ((sw_tHdr[5] << 16) | sw_tHdr[4]);
			return tmp;
		};
		uint32_t triggerNumber() const
		{
			uint32_t tmp = ((sw_tHdr[7] << 16) | sw_tHdr[6]);
			return tmp;
		};
		uint16_t mode() const { return (sw_tHdr[8] & 0b1111000000000000) >> 12; };
		uint16_t channel() const { return (sw_tHdr[8] & 0b0000111100000000) >> 8; };
		uint16_t type() const { return (sw_tHdr[8] & 0b0000000011111111); };
		uint64_t triggerTime() const
		{
			uint64_t T1 = sw_tHdr[12], T2 = sw_tHdr[11], T3 = sw_tHdr[10], T4 = sw_tHdr[9], time;
			T1 = T1 << 48;
			T2 = T2 << 32;
			T3 = T3 << 16;
			time = T1 | T2 | T3 | T4;
			return time;
		};
		uint32_t adcOffset() const
		{
			uint32_t tmp = (sw_tHdr[14] << 16) | sw_tHdr[13];
			return tmp;
		};
		uint16_t droppedPackets() const { return sw_tHdr[15]; }
		uint64_t unixTime() const
		{
			uint64_t T1 = sw_tHdr[19], T2 = sw_tHdr[18], T3 = sw_tHdr[17], T4 = sw_tHdr[16], time;
			T1 = T1 << 48;
			T2 = T2 << 32;
			T3 = T3 << 16;
			time = T1 | T2 | T3 | T4;
			return time;
		};  // Epoch time in milliseconds.
	};

	class STM_sHdr
	{
	public:
		uint16_t sw_sHdr[8];
		STM_sHdr(){};

		// Reverse order - data is little endian.
		uint32_t sliceNumber() const
		{
			uint32_t tmp = (sw_sHdr[1] << 16) | sw_sHdr[0];
			return tmp;
		};
		uint32_t sliceSize() const
		{
			uint32_t tmp = (sw_sHdr[3] << 16) | sw_sHdr[2];
			return tmp;
		};
		uint64_t adcTime() const
		{
			uint64_t T1 = sw_sHdr[7], T2 = sw_sHdr[6], T3 = sw_sHdr[5], T4 = sw_sHdr[4], time;
			T1 = T1 << 48;
			T2 = T2 << 32;
			T3 = T3 << 16;
			time = T1 | T2 | T3 | T4;
			return time;
		};
	};

	STM_tHdr const* GetTHdr()
	{
		return reinterpret_cast<STM_tHdr const*>(artdaq_fragment_.dataBegin());
	}

	STM_sHdr const* GetSHdr() {
		return reinterpret_cast<STM_sHdr const*>(GetTHdr() + 1);
	}

	uint16_t const* DataBegin() {
		return reinterpret_cast<uint16_t const*>(GetSHdr() + 1);
	}

	uint16_t const* DataEnd() {
		return DataBegin() + GetSHdr()->sliceSize();
	}

	//	std::unique_ptr<STMBoardID> GetSTMBoardID(size_t blockIndex) const;
	//	std::vector<std::pair<STMHitReadoutPacket, std::vector<uint16_t>>> GetSTMHits(size_t blockIndex) const;
	//	std::vector<std::pair<STMHitReadoutPacket, uint16_t>> GetSTMHitsForTrigger(size_t blockIndex) const;

private:
	artdaq::Fragment const& artdaq_fragment_;
};

/*
std::ostream& operator<<(std::ostream& os, const STMFragment::STMDataPacket& data) {
  os << std::setw(4) << std::setfill('0') << std::right << std::hex << data.FixedHeaderWord[0] << ", " << data.FixedHeaderWord[1] << ", " << data.FixedHeaderWord[2] << ", " << data.FixedHeaderWord[3] << ", " << data.FixedHeaderWord[4] << ", " << data.FixedHeaderWord[5] << ", " << data.FixedHeaderWord[6] << ", " << data.FixedHeaderWord[7] << std::endl;
  return os;
}
*/

static constexpr int sw_tHdr_size_bytes = sizeof(STMFragment::STM_tHdr);
static constexpr int sw_tHdr_size_words = sw_tHdr_size_bytes / 2;
static constexpr int sw_sHdr_size_bytes = sizeof(STMFragment::STM_sHdr);
static constexpr int sw_sHdr_size_words = sw_sHdr_size_bytes / 2;

}  // namespace mu2e

/*    uint16_t sw_tHdr[20];
uint16_t sw_sHdr[8]; //Call this from XML? Worth calling the parameter indexes from XML too?
bool recombine = false;
uint32_t sw_tHdr_dataSize = (sw_tHdr_dataSize1 << 16) | sw_tHdr_dataSize2;
uint32_t sw_tHdr_sliceNumber = (sw_tHdr_sNum1 << 16)| sw_tHdr_sNum2;*/

//    STMDataPacket() : FixedHeaderWord0(0), FixedHeaderWord1(0), FixedHeaderWord2(0), FixedHeaderWord3(0), FixedHeaderWord4(0), FixedHeaderWord5(0), FixedHeaderWord6(0), FixedHeaderWord7(0) { }
// const std::size_t sw_tHdr_size = 20; // Number of words (uint16_t) in the trigger header.
// const std::size_t sw_sHdr_size = 8; //Number of words (uint16_t) in the slice header.

/*
  // Add to the STMDataPacket definition
  uint16_t sw_tHdr[20];
  uint16_t sw_sHdr[8]; //Call this from XML? Worth calling the parameter indexes from XML too?
  bool recombine = false;
  // Software trigger header members
  uint16_t sw_tHdr_size = 20; // Number of words (uint16_t) in the trigger header.
  uint16_t* sw_tHdr_testWord0 = &sw_tHdr[0];
  uint16_t* sw_tHdr_testWord1 = &sw_tHdr[1];
  uint16_t* sw_tHdr_dataSize1 = &sw_tHdr[2];
  uint16_t* sw_tHdr_dataSize2 = &sw_tHdr[3];
  uint16_t* sw_tHdr_sNum1 = &sw_tHdr[4];
  uint16_t* sw_tHdr_sNum2 = &sw_tHdr[5];
  uint16_t* sw_tHdr_TrigNum1 = &sw_tHdr[6];
  uint16_t* sw_tHdr_TrigNum2 = &sw_tHdr[7];
  uint16_t* sw_tHdr_MdChTp = &sw_tHdr[8];
  uint16_t* sw_tHdr_TrigTime1 = &sw_tHdr[9];
  uint16_t* sw_tHdr_TrigTime2 = &sw_tHdr[10];
  uint16_t* sw_tHdr_TrigTime3 = &sw_tHdr[11];
  uint16_t* sw_tHdr_TrigTime4 = &sw_tHdr[12];
  uint16_t* sw_tHdr_ADCoffset1 = &sw_tHdr[13];
  uint16_t* sw_tHdr_ADCoffset2 = &sw_tHdr[14];
  uint16_t* sw_tHdr_DroppedPackets = &sw_tHdr[15];
  uint16_t* sw_tHdr_unixTime1 = &sw_tHdr[16];
  uint16_t* sw_tHdr_unixTime2 = &sw_tHdr[17];
  uint16_t* sw_tHdr_unixTime3 = &sw_tHdr[18];
  uint16_t* sw_tHdr_unixTime4 = &sw_tHdr[19];
  uint32_t sw_tHdr_dataSize = (&sw_tHdr_dataSize1 << 16) | &sw_tHdr_dataSize2;
  uint32_t sw_tHdr_sliceNumber = (&sw_tHdr_sNum1 << 16)| &sw_tHdr_sNum2;

  // Recombine words
  if(recombine){
	uint32_t sw_tHdr_testData = (&sw_tHdr_testWord0 << 16) | &sw_tHdr_testWord1;
	//uint32_t sw_tHdr_dataSize = (&sw_tHdr_dataSize1 << 16) | &sw_tHdr_dataSize2;
	//uint32_t sw_tHdr_sliceNumber = (&sw_tHdr_sNum1 << 16)| &sw_tHdr_sNum2;
	uint4_t sw_tHdr_mode = &sw_tHdr_MdChTp & 0b1111000000000000; // Check this
	uint4_t sw_tHdr_channelNumber = &sw_tHdr_MdChTp & 0b0000111100000000; // Check this
	uint8_t sw_tHdr_type = &sw_tHdr_MdChTp & 0b0000000011111111; // Check this
	uint32_t sw_tHdr_ADCOffset = (&sw_tHdr_ADCOffset1 << 16) | &sw_tHdr_ADCOffset2;
	uint64_t sw_tHdr_unixTime = (&sw_tHdr_unixTime1 << 48) | (&sw_tHdr_unixTime2 << 32) | (&sw_tHdr_unixTime3 << 16) | &sw_tHdr_unixTime4;
  };

  // Software slice header members
  uint16_t sw_sHdr_size = 8; // Number of words (uint16_t) in the slice header.
  uint16_t* sw_sHdr_sNum1 = &sw_sHdr[0];
  uint16_t* sw_sHdr_sNum2 = &sw_sHdr[1];
  uint16_t* sw_sHdr_sSize1 = &sw_sHdr[2];
  uint16_t* sw_sHdr_sSize2 = &sw_sHdr[3];
  uint16_t* sw_sHdr_ADCtime1 = &sw_sHdr[4];
  uint16_t* sw_sHdr_ADCtime2 = &sw_sHdr[5];
  uint16_t* sw_sHdr_ADCtime3 = &sw_sHdr[6];
  uint16_t* sw_sHdr_ADCtime4 = &sw_sHdr[7];
  uint32_t sw_sHdr_sliceSize = (&sw_sHdr_sSize1 << 16) | &sw_sHdr_sSize2;
  // Recombine words
  if(recombine)}{
	uint32_t sw_sHdr_sliceNumber = (&sw_sHdr_sNum1 << 16) | &sw_sHdr_sNum2;
	//uint32_t sw_sHdr_sliceSize = (&sw_sHdr_sSize1 << 16) | &sw_sHdr_sSize2;
	uint64_t sw_sHdr_ADCtime = (&sw_sHdr_ADCtime1 << 48) | (&sw_sHdr_ADCtime2 << 32) | (&sw_sHdr_ADCtime3 << 16) | &sw_sHdr_ADCtime4;
  };
  //uint16_t STMDataPacketSize = 2000; //Each WireShark packet is 2kB.
  */

/*
  void update_tHdr_members(STMDataPacket &packet){
	packet->sw_tHdr_testWord0 = &sw_tHdr[0];
	packet->sw_tHdr_testWord1 = &sw_tHdr[1];
	packet->sw_tHdr_dataSize1 = &sw_tHdr[2];
	packet->sw_tHdr_dataSize2 = &sw_tHdr[3];
	packet->sw_tHdr_sNum1 = &sw_tHdr[4];
	packet->sw_tHdr_sNum2 = &sw_tHdr[5];
	packet->sw_tHdr_TrigNum1 = &sw_tHdr[6];
	packet->sw_tHdr_TrigNum2 = &sw_tHdr[7];
	packet->sw_tHr_MdChTp = &sw_tHdr[8];
	packet->sw_tHdr_TrigTime1 = &sw_tHdr[9];
	packet->sw_tHdr_TrigTime2 = &sw_tHdr[10];
	packet->sw_tHdr_TrigTime3 = &sw_tHdr[11];
	packet->sw_tHdr_TrigTime4 = &sw_tHdr[12];
	packet->sw_tHdr_ADCoffset1 = &sw_tHdr[13];
	packet->sw_tHdr_ADCoffset2 = &sw_tHdr[14];
	packet->sw_tHdr_DroppedPackets = &sw_tHdr[15];
	packet->sw_tHdr_unixTime1 = &sw_tHdr[16];
	packet->sw_tHdr_unixTime2 = &sw_tHdr[17];
	packet->sw_tHdr_unixTime3 = &sw_tHdr[18];
	packet->sw_tHdr_unixTime4 = &sw_tHdr[19];
	packet->sw_tHdr_dataSize = (packet->sw_tHdr_dataSize1 << 16) | packet->sw_tHdr_dataSize2;
	packet->sw_tHdr_sliceNumber = (packet->sw_tHdr_sNum1 << 16)| packet->sw_tHdr_sNum2;
	if(packet->recombine){
	  packet->sw_tHdr_testData = (packet->sw_tHdr_testWord0 << 16) | packet->sw_tHdr_testWord1;
	  //packet->sw_tHdr_dataSize = (packet->sw_tHdr_dataSize1 << 16) | packet->sw_tHdr_dataSize2;
	  //packet->sw_tHdr_sliceNumber = (packet->sw_tHdr_sNum1 << 16)| packet->sw_tHdr_sNum2;
	  packet->sw_tHdr_mode = packet->sw_tHdr_MdChTp & 0b1111000000000000; // Check this
	  packet->sw_tHdr_channelNumber = packet->sw_tHdr_MdChTp & 0b0000111100000000; // Check this
	  packet->sw_tHdr_type = packet->sw_tHdr_MdChTp & 0b0000000011111111; // Check this
	  packet->sw_tHdr_ADCOffset = (packet->sw_tHdr_ADCOffset1 << 16) | packet->sw_tHdr_ADCOffset2;
	  packet->sw_tHdr_unixTime = (packet->sw_tHdr_unixTime1 << 48) | (packet->sw_tHdr_unixTime2 << 32) | (packet->sw_tHdr_unixTime3 << 16) | packet->sw_tHdr_unixTime4;
	};};

  void update_sHdr_members(STMDataPacket &packet){
	packet->s w_sHdr_sNum1 = &sw_sHdr[0];
	packet->sw_sHdr_sNum2 = &sw_sHdr[1];
	packet->sw_sHdr_sSize1 = &sw_sHdr[2];
	packet->sw_sHdr_sSize2 = &sw_sHdr[3];
	packet->sw_sHdr_ADCtime1 = &sw_sHdr[4];
	packet->sw_sHdr_ADCtime2 = &sw_sHdr[5];
	packet->sw_sHdr_ADCtime3 = &sw_sHdr[6];
	packet->sw_sHdr_ADCtime4 = &sw_sHdr[7];
	// Recombine words
	if(packet->recombine){
	  packet->sw_sHdr_sliceNumber = (packet->sw_sHdr_sNum1 << 16) | packet->sw_sHdr_sNum2;
	  packet->sw_sHdr_sliceSize = (packet->sw_sHdr_sSize1 << 16) | packet->sw_sHdr_sSize2;
	  packet->sw_sHdr_ADCtime = (packet->sw_sHdr_ADCtime1 << 48) | (packet->sw_sHdr_ADCtime2 << 32) | (packet->sw_sHdr_ADCtime3 << 16) | packet->sw_sHdr_ADCtime4;
	};};
*/

#endif  // MU2E_ARTDAQ_CORE_OVERLAYS_STMFRAGMENT_HH
