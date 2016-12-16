#ifndef mu2e_artdaq_core_Overlays_DataBlockIterator_hh
#define mu2e_artdaq_core_Overlays_DataBlockIterator_hh

///////////////////////////////////////////////////////////////////////
// DataBlockIterator
//
// Helper class which should allow STL algorithms to work over DataBlocks
//
///////////////////////////////////////////////////////////////////////

#include "mu2e-artdaq-core/Overlays/DTCFragment.hh"
#include "mu2e-artdaq-core/Overlays/DataBlock.hh"

#include <cstdint>
#include <cstddef>
#include <iterator>

namespace mu2e {
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
