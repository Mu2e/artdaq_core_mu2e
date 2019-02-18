#include "mu2e-artdaq-core/Overlays/ArtFragmentReader.hh"

#include <iomanip>

std::ostream& mu2e::operator<<(std::ostream& os, ArtFragmentReader const& f) {
  os << "ArtFragmentReader " << std::dec << ", block count: " << f.block_count() << "\n";
  return os;
}
