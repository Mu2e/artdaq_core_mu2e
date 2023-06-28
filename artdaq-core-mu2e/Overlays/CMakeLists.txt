cet_make_library(LIBRARY_NAME Overlays
SOURCE 
FragmentType.cc 
DTCEventFragment.cc
STMFragment.cc
  LIBRARIES PUBLIC
  mu2e_pcie_utils::DTCInterface
  artdaq_core::artdaq-core_Data
  )

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

include(ArtdaqCorePlugins)

  cet_build_plugin(Mu2e artdaq::fragmentNameHelper LIBRARIES REG  artdaq_core_mu2e::Overlays )


install_headers()
install_source()
