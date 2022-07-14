#ifndef mu2e_artdaq_core_Overlays_mu2eFragmentReader_hh
#define mu2e_artdaq_core_Overlays_mu2eFragmentReader_hh

////////////////////////////////////////////////////////////////////////
// mu2eFragmentReader
//
// Class derived from mu2eFragment which provides additional methods
// for accessing data stored in a mu2eFragment
//
////////////////////////////////////////////////////////////////////////

#include "artdaq-core/Data/Fragment.hh"
#include "artdaq-core/Data/Fragments.hh"
#include "mu2e-artdaq-core/Overlays/mu2eFragment.hh"

#include <bitset>

#include <iostream>

namespace mu2e {
class mu2eFragmentReader;
}

class mu2e::mu2eFragmentReader : public mu2e::mu2eFragment
{
public:
	typedef uint16_t adc_t;

	mu2eFragmentReader(artdaq::Fragment const &f)
		: mu2eFragment(f){};

	size_t blockIndexBytes(size_t offset) const;

	size_t blockSizeBytes() const;
	size_t blockSizeBytes(size_t offset) const;
	size_t blockEndBytes(size_t offset) const;
	Header::data_t const *dataAtBytes(size_t offset) const;
	Header::data_t const *dataAtBlockIndex(size_t offset) const;

	void printPacketAtByte(size_t offset) const;

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
	uint32_t DBT_TDC0(adc_t const *pos);
	uint32_t DBT_TDC1(adc_t const *pos);
	uint32_t DBT_TOT0(adc_t const *pos);
	uint32_t DBT_TOT1(adc_t const *pos);
	std::array<adc_t, 15> DBT_Waveform(adc_t const *pos);
	adc_t DBT_Flags(adc_t const *pos);

	// CAL DataBlock Payload Accessor Methods (by block address)
	adc_t DBC_CrystalID(adc_t const *pos);
	adc_t DBC_apdID(adc_t const *pos);
	adc_t DBC_Time(adc_t const *pos);
	adc_t DBC_NumSamples(adc_t const *pos);
	adc_t DBC_PeakSampleIdx(adc_t const *pos);
	std::vector<adc_t> DBC_Waveform(adc_t const *pos);

protected:
	// Bit processing helper methods
	// For now, the focus is on clarity and ease of use, but eventually
	// these sorts of functions will need to be optimized for efficiency

	// bitArray populates a 128-bit bitset using the 128 bits beginning
	// at the position indicated by the provided pointer
	//  std::bitset<128> bitArray(adc_t const * beginning);
	std::bitset<128> bitArray(adc_t const *beginning) const;

	// Populates the provided bitset using the 128 bits beginning at the
	// position indicated by the provided pointer
	void fillBitArray(std::bitset<128> &theArray, adc_t const *beginning);

	void printBitArray(std::bitset<128> theArray);

	// Accepts a 128 bit bitset and converts the bits from minIdx to maxIdx
	// into a 16-bit adc_t (minIdx and maxIdx should be within 16 bits of
	// each other as no error-checking is performed at the moment).
	adc_t convertFromBinary(std::bitset<128> theArray, int minIdx, int maxIdx) const;
};

// Convert index in number of DataBlocks to index in bytes
inline size_t mu2e::mu2eFragmentReader::blockIndexBytes(size_t offset) const
{
	if (hdr_block_count() == 0) {
		return 0;
	}
	else if (offset >= hdr_block_count())
	{
		return -1;
	}
	else if (offset == 0)
	{
		return 0;
	}
	return header_()->index[offset - 1];
}

// Return size of all blocks
inline size_t mu2e::mu2eFragmentReader::blockSizeBytes() const { return mu2eFragment::dataEndBytes(); }

// Return size of block at given DataBlock index
inline size_t mu2e::mu2eFragmentReader::blockSizeBytes(size_t offset) const
{
	if (hdr_block_count() == 0) {
		return 0;
	}
	else if (offset > hdr_block_count() - 1)
	{
		return 0;
	}
	else if (offset == 0)
	{
		return header_()->index[offset];
	}
	return header_()->index[offset] - header_()->index[offset - 1];
}

// Return size of block at given DataBlock index
inline size_t mu2e::mu2eFragmentReader::blockEndBytes(size_t offset) const
{
	if (hdr_block_count() == 0) {
		return 0;
	}
	else if (offset > hdr_block_count() - 1)
	{
		return 0;
	}
	return header_()->index[offset];
}

// Return pointer to beginning of DataBlock at given byte index
inline mu2e::mu2eFragmentReader::Header::data_t const *mu2e::mu2eFragmentReader::dataAtBytes(size_t offset) const
{
	return dataBegin() + (offset / sizeof(Header::data_t));
}

// Return pointer to beginning of DataBlock at given DataBlock index
inline mu2e::mu2eFragmentReader::Header::data_t const *mu2e::mu2eFragmentReader::dataAtBlockIndex(size_t offset) const
{
	return dataAtBytes(blockIndexBytes(offset));
}

void mu2e::mu2eFragmentReader::printPacketAtByte(size_t offset) const
{
	std::bitset<128> theArray = bitArray(reinterpret_cast<mu2e::mu2eFragmentReader::adc_t const *>(dataAtBytes(offset)));
	std::cout << "\t\t"
			  << "Packet Bits (128): ";
	for (int i = 0; i < 128; i++) {
		std::cout << theArray[i];
		if (i != 0 && i < 128 - 1 && (i + 1) % 8 == 0) {
			std::cout << " ";
		}
	}
	std::cout << std::endl;
	return;
}

std::bitset<128> mu2e::mu2eFragmentReader::bitArray(mu2e::mu2eFragmentReader::adc_t const *beginning) const
{
	// Return 128 bit bitset filled with bits starting at the indicated position in the fragment
	std::bitset<128> theArray;
	for (int bitIdx = 127, adcIdx = 0; adcIdx < 8; adcIdx++) {
		for (int offset = 0; offset < 16; offset++) {
			if (((*((adc_t const *)(beginning + adcIdx))) & (1 << offset)) != 0) {
				theArray.set(bitIdx);
			}
			else
			{
				theArray.reset(bitIdx);
			}
			bitIdx--;
		}
	}
	return theArray;
}

void mu2e::mu2eFragmentReader::fillBitArray(std::bitset<128> &theArray,
											mu2e::mu2eFragmentReader::adc_t const *beginning)
{
	// Fill bitset using the 128 bits starting at the indicated position in the fragment
	for (int bitIdx = 127, adcIdx = 0; adcIdx < 8; adcIdx++) {
		for (int offset = 0; offset < 16; offset++) {
			if (((*((adc_t const *)(beginning + adcIdx))) & (1 << offset)) != 0) {
				theArray.set(bitIdx);
			}
			else
			{
				theArray.reset(bitIdx);
			}
			bitIdx--;
		}
	}
}

void mu2e::mu2eFragmentReader::printBitArray(std::bitset<128> theArray)
{
	// Print all 128 bits in the packet
	std::cout << "\t\t"
			  << "Packet Bits (128): ";
	for (int i = 0; i < 128; i++) {
		std::cout << theArray[i];
	}
	std::cout << std::endl;
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::convertFromBinary(std::bitset<128> theArray, int minIdx,
																			int maxIdx) const
{
	std::bitset<16> retVal;
	for (int i = minIdx + 1; i <= maxIdx; i++) {
		retVal.set(maxIdx - i, theArray[i]);
	}
	return retVal.to_ulong();
}

////////////////////////////////////////////////////////////////////////////////
// DataBlock Header Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_ByteCount(adc_t const *pos) { return *(pos + 0); }

bool mu2e::mu2eFragmentReader::DBH_Valid(adc_t const *pos) { return (*(pos + 1) >> 15) & 0x0001; }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_ROCID(adc_t const *pos)
{
	return *(pos + 1) & 0x000F;  // 0x000F = 0b1111
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_PacketType(adc_t const *pos)
{
	return (*(pos + 1) >> 4) & 0x000F;  // 0x000F = 0b1111
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_PacketCount(adc_t const *pos)
{
	return *(pos + 2) & 0x07FF;  // 0x07FF = 0b0111 1111 1111
}

uint64_t mu2e::mu2eFragmentReader::DBH_Timestamp(adc_t const *pos)
{
	return uint64_t(*(pos + 3)) + (uint64_t(*(pos + 4)) << 16) + (uint64_t(*(pos + 5)) << 32);
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_TimestampLow(adc_t const *pos) { return *(pos + 3); }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_TimestampMedium(adc_t const *pos) { return *(pos + 4); }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_TimestampHigh(adc_t const *pos) { return *(pos + 5); }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_Status(adc_t const *pos)
{
	return *(pos + 6) & 0x00FF;  // 0x00FF = 0b1111 1111
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_FormatVersion(adc_t const *pos)
{
	return *(pos + 6) >> 8;
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_EVBMode(adc_t const *pos) { return *(pos + 7) >> 8; }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_SubsystemID(adc_t const *pos)
{
	return (*(pos + 7) >> 6) & 0x0003;  // 0x0003 = 0b0011
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBH_DTCID(adc_t const *pos)
{
	return *(pos + 7) & 0x003F;  // 0x003F = 0b0011 1111
}

////////////////////////////////////////////////////////////////////////////////
// TRK DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////
mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBT_StrawIndex(adc_t const *pos) { return *(pos + 8 + 0); }

uint32_t mu2e::mu2eFragmentReader::DBT_TDC0(adc_t const *pos) { return (uint32_t(*(pos + 8 + 1)) & 0xFFFF); }

uint32_t mu2e::mu2eFragmentReader::DBT_TDC1(adc_t const *pos) { return (uint32_t(*(pos + 8 + 2)) & 0xFFFF); }

uint32_t mu2e::mu2eFragmentReader::DBT_TOT0(adc_t const *pos) { return (uint32_t(*(pos + 8 + 3)) & 0x00FF); }

uint32_t mu2e::mu2eFragmentReader::DBT_TOT1(adc_t const *pos) { return ((uint32_t(*(pos + 8 + 3)) >> 8) & 0x00FF); }

std::array<mu2e::mu2eFragmentReader::adc_t, 15> mu2e::mu2eFragmentReader::DBT_Waveform(adc_t const *pos)
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

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBT_Flags(adc_t const *pos) { return (*(pos + 8 + 15) >> 8); }

////////////////////////////////////////////////////////////////////////////////
// CAL DataBlock Payload Accessor Methods (by block address)
////////////////////////////////////////////////////////////////////////////////

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBC_CrystalID(adc_t const *pos)
{
	return *(pos + 8 + 0) & 0x0FFF;  // 0x0FFF = 0b1111 1111 1111
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBC_apdID(adc_t const *pos) { return *(pos + 8 + 0) >> 12; }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBC_Time(adc_t const *pos) { return *(pos + 8 + 1); }

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBC_NumSamples(adc_t const *pos)
{
	return *(pos + 8 + 2) & 0x00FF;
}

mu2e::mu2eFragmentReader::adc_t mu2e::mu2eFragmentReader::DBC_PeakSampleIdx(adc_t const *pos)
{
	return *(pos + 8 + 2) >> 8;
}

std::vector<mu2e::mu2eFragmentReader::adc_t> mu2e::mu2eFragmentReader::DBC_Waveform(adc_t const *pos)
{
	std::vector<int> waveform(DBC_NumSamples(pos));
	for (size_t i = 0; i < waveform.size(); i++) {
		waveform[i] = *(pos + 8 + 3 + i);
	}
	return waveform;
}

#endif /* mu2e_artdaq_Overlays_mu2eFragmentReader_hh */
