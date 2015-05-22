#ifndef mu2e_artdaq_core_BuildInfo_GetPackageBuildInfo_hh
#define mu2e_artdaq_core_BuildInfo_GetPackageBuildInfo_hh

#include "artdaq-core/Data/PackageBuildInfo.hh"

#include <string>

namespace mu2ecore {

  struct GetPackageBuildInfo {

    static artdaq::PackageBuildInfo getPackageBuildInfo();
  };

}

#endif /* mu2e_artdaq_core_BuildInfo_GetPackageBuildInfo_hh */
