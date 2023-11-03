cet_make_library(
    SOURCE 
      FragmentType.cc 
      DTCEventFragment.cc
      STMFragment.cc
      DTC_Packets.cpp
      DTC_Types.cpp
    LIBRARIES PUBLIC
      artdaq_core::artdaq-core_Data
)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

include(ArtdaqCorePlugins)

  cet_build_plugin(Mu2e artdaq::fragmentNameHelper LIBRARIES REG  artdaq_core_mu2e::artdaq-core-mu2e_Overlays )


install_headers()
install_source()
