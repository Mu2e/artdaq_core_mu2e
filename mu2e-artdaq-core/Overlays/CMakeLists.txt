cet_make(NO_DICTIONARY 
  EXCLUDE Mu2e_fragmentNameHelper.cc
  LIBRARIES
  pcie_linux_kernel_module::DTCInterface
  artdaq_core::artdaq-core_Data
  )

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()


  basic_plugin(Mu2e "fragmentNameHelper"  
  mu2e_artdaq_core::mu2e-artdaq-core_Overlays
  artdaq_core::artdaq-core_Data_Artdaq_fragmentNameHelper
 )

cet_remove_compiler_flags(-fsanitize=address -lasan)
cet_remove_compiler_flags(-fsanitize=thread -ltsan)
  art_dictionary(DICTIONARY_LIBRARIES mu2e-artdaq-core_Overlays)

install_headers()
install_source()
