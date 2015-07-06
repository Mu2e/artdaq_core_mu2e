#ifndef mu2e_artdaq_core_Overlays_DataBlock_hh
#define mu2e_artdaq_core_Overlays_DataBlock_hh

///////////////////////////////////////////////////////////////////////
// DataBlock
//
// Class representing a mu2e "DataBlock", or DataHeaderPacket and its
// associated DataPackets. Each Fragment is made up of all DataBlocks
// from the ROCs connected to one DTC for one timestamp.
//
///////////////////////////////////////////////////////////////////////

#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"

#include <cstdint>
#include <cstddef>
#include <iterator>

namespace mu2e {
class DataBlock {
public:
  DataBlock();
  DataBlock(packet_t* ptr);
  virtual ~DataBlock() {}
 
  packet_t* headerPacket() { return header_; }
  packet_t* dataBegin() { return data_; }
  packet_t* dataEnd() { return data_ + getPacketCount(); }
  packet_t* dataPacket(uint16_t index);

  uint16_t getPacketCount();
  uint8_t getROCID();

private:
  packet_t* header_;
  packet_t* data_;

}; // class DataBlock

class DataBlockIterator {
  typedef std::ptrdiff_t difference_type;
  typedef DataBlock value_type;
  typedef typename DataBlock* pointer;
  typedef typename DataBlock& reference;
  typedef std::forward_iterator_tag iterator_category;
  
  DataBlockIterator(pointer x = nullptr) : p(x) { }
  DataBlockIterator(const DataBlockIterator& i) 
    : p(i.p) { }
  reference operator*() const { return *p; }
  pointer operator->() const { return p; }
  DataBlockIterator& operator++() { 
    DataBlock* pN = new DataBlock(p->dataEnd());
    delete p;
    p = pN;
    return *this; 
  }
  DataBlockIterator& operator++(int) {
    DataBlockIterator tmp(*this);
    ++*this;
    return tmp;
  }
  
private:
  pointer p;
};

} // namespace mu2e

#endif
