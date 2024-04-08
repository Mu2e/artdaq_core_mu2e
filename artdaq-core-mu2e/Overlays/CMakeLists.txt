cet_make_library(
    SOURCE 
      FragmentType.cc 
      DTCEventFragment.cc
      STMFragment.cc
      CFO_Packets/CFO_DataPacket.cpp
      CFO_Packets/CFO_DMAPacket.cpp
      CFO_Packets/CFO_Event.cpp
      DTC_Packets/DTC_DataHeaderPacket.cpp
      DTC_Packets/DTC_DataPacket.cpp
      DTC_Packets/DTC_DataRequestPacket.cpp
      DTC_Packets/DTC_DCSReplyPacket.cpp
      DTC_Packets/DTC_DCSRequestPacket.cpp
      DTC_Packets/DTC_DMAPacket.cpp
      DTC_Packets/DTC_Event.cpp
      DTC_Packets/DTC_HeartbeatPacket.cpp
      DTC_Packets/DTC_SubEvent.cpp
      DTC_Types/DTC_CharacterNotInTableError.cpp
      DTC_Types/DTC_DebugType.cpp
      DTC_Types/DTC_EventWindowTag.cpp
      DTC_Types/DTC_RXStatus.cpp
      DTC_Types/DTC_SERDESRXDisparityError.cpp
      DTC_Types/DTC_SimMode.cpp
      DTC_Types/Utilities.cpp
    LIBRARIES PUBLIC
      artdaq_core::artdaq-core_Data
)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

include(ArtdaqCorePlugins)

  cet_build_plugin(Mu2e artdaq::fragmentNameHelper LIBRARIES REG  artdaq_core_mu2e::artdaq-core-mu2e_Overlays )


install_headers(SUBDIRS CFO_Packets DTC_Packets DTC_Types)
install_source(SUBDIRS CFO_Packets DTC_Packets DTC_Types)
