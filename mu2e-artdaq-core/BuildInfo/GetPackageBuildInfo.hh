#ifndef mu2e_artdaq_core_BuildInfo_GetPackageBuildInfo_hh
#define mu2e_artdaq_core_BuildInfo_GetPackageBuildInfo_hh

#include "artdaq-core/Data/PackageBuildInfo.hh"

#include <string>

namespace mu2e {
	/// <summary>
	/// Gets the Build information for the mu2e_artdaq_core package
	/// </summary>
  struct GetPackageBuildInfo {
	  /// <summary>
	  /// Determine the Build information for this package, including version number and build timestamp
	  /// </summary>
	  /// <returns>artdaq::PackageBuildInfo object containing build information</returns>
    static artdaq::PackageBuildInfo getPackageBuildInfo();
  };

}

#endif /* mu2e_artdaq_core_BuildInfo_GetPackageBuildInfo_hh */
