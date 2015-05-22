#include "mu2e-artdaq-core/Overlays/FragmentType.hh"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

namespace {
  static std::vector<std::string> const
  names { "MISSED", "TOY1", "TOY2", "UNKNOWN" };
}

mu2ecore::FragmentType
mu2ecore::toFragmentType(std::string t_string)
{
  std::transform(t_string.begin(),
                 t_string.end(),
                 t_string.begin(),
                 toupper);
  auto it = std::find(names.begin(), names.end(), t_string);
  return (it == names.end()) ?
    FragmentType::INVALID :
    static_cast<FragmentType>(artdaq::Fragment::FirstUserFragmentType +
                              (it - names.begin()));
}

std::string
mu2ecore::fragmentTypeToString(FragmentType val)
{
  if (val < FragmentType::INVALID) {
    return names[val - FragmentType::MISSED];
  }
  else {
    return "INVALID/UNKNOWN";
  }
}
