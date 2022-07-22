#ifndef artdaq_core_mu2e_BuildInfo_GetPackageBuildInfo_hh
#define artdaq_core_mu2e_BuildInfo_GetPackageBuildInfo_hh

#include "artdaq-core/Data/PackageBuildInfo.hh"

#include <string>

namespace mu2e {
/// <summary>
/// Gets the Build information for the artdaq_core_mu2e package
/// </summary>
struct GetPackageBuildInfo
{
	/// <summary>
	/// Determine the Build information for this package, including version number and build timestamp
	/// </summary>
	/// <returns>artdaq::PackageBuildInfo object containing build information</returns>
	static artdaq::PackageBuildInfo getPackageBuildInfo();
};

}  // namespace mu2e

#endif /* artdaq_core_mu2e_BuildInfo_GetPackageBuildInfo_hh */
