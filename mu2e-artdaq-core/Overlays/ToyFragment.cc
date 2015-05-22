#include "mu2e-artdaq-core/Overlays/ToyFragment.hh"

#include "cetlib/exception.h"

// 26-Sep-2014, KAB: commenting out this function since it is unused
// and the compiler doesn't like that
#if 0
namespace {
  unsigned int pop_count (unsigned int n) {
    unsigned int c; 
    for (c = 0; n; c++) n &= n - 1; 
    return c;
  }
}
#endif

void mu2ecore::ToyFragment::checkADCData(int daq_adc_bits) const {
  mu2ecore::ToyFragment::adc_t const * adcPtr(findBadADC(daq_adc_bits));
  if (adcPtr != dataEnd()) {
    throw cet::exception("IllegalADCVal")
        << "Illegal value of ADC word #"
        << (adcPtr - dataBegin())
        << ": 0x"
        << std::hex
        << *adcPtr
        << ".";
  }
}

std::ostream & mu2ecore::operator << (std::ostream & os, ToyFragment const & f) {
  os << "ToyFragment event size: "
     << f.hdr_event_size()
     << ", run number: "
     << f.hdr_run_number()
     << "\n";

  return os;
}

