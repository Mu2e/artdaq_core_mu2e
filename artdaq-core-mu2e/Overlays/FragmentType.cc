#include "artdaq-core-mu2e/Overlays/FragmentType.hh"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

mu2e::FragmentType mu2e::toFragmentType(std::string t_string)
{
	std::transform(t_string.begin(), t_string.end(), t_string.begin(), toupper);
	for (auto& it : names)
	{
		if (it.second == t_string) return it.first;
	}
	return FragmentType::INVALID;
}

std::string mu2e::fragmentTypeToString(FragmentType val)
{
        if (mu2e::names.count(val))
	{
	        return mu2e::names.at(val);
	}

	return "INVALID/UNKNOWN";
}

std::map<artdaq::Fragment::type_t, std::string> mu2e::makeFragmentTypeMap()
{
	auto output = artdaq::Fragment::MakeSystemTypeMap();
	for (const auto& name : mu2e::names)
	{
		output[name.first] = name.second;
	}
	return output;
}
