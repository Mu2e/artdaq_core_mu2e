cet_make_library(LIBRARY_NAME Overlays
SOURCE 
ArtFragment.cc 
CalorimeterFragment.cc 
CRVFragment.cc 
DTCFragment.cc 
FragmentType.cc 
Mu2eEventFragmentWriter.cc 
Mu2eEventHeader.cc
mu2eFragment.cc
TrackerFragment.cc
  LIBRARIES PUBLIC
  pcie_linux_kernel_module::DTCInterface
  artdaq_core::artdaq-core_Data
  )

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

include(ArtdaqCorePlugins)

  cet_build_plugin(Mu2e artdaq::fragmentNameHelper LIBRARIES REG  artdaq_core_mu2e::Overlays )

  
include(ArtDictionary)

art_dictionary(DICTIONARY_LIBRARIES
  artdaq_core_mu2e::Overlays
)

get_target_property(DictCompileOptions artdaq-core-mu2e_Overlays_dict COMPILE_OPTIONS)
if(DictCompileOptions)
	string(REPLACE "-fsanitize=thread" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-fsanitize=address" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-ltsan" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-lasan" "" DictCompileOptions "${DictCompileOptions}")

	set_target_properties(artdaq-core-mu2e_Overlays_dict PROPERTIES COMPILE_OPTIONS "${DictCompileOptions}")
endif()

install_headers()
install_source()
