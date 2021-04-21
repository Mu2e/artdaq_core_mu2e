cet_make(NO_DICTIONARY 
  EXCLUDE Mu2e_fragmentNameHelper.cc
  LIBRARIES
 ${ARTDAQ_CORE_PREFIX}artdaq-core_Utilities
  pcie_linux_kernel_module::DTCInterface
  ${CETLIB_PREFIX}cetlib
  ${CETLIB_EXCEPT_PREFIX}cetlib_except
    ${MF_MESSAGELOGGER}
  )

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()


  basic_plugin(Mu2e "fragmentNameHelper"
   ${ARTDAQ_CORE_PREFIX}artdaq-core_Data
    mu2e-artdaq-core_Overlays
    ${ARTDAQ_CORE_PREFIX}artdaq-core_Data_Artdaq_fragmentNameHelper
    ${MF_MESSAGELOGGER}
 )

cet_remove_compiler_flags(-fsanitize=address -lasan)
cet_remove_compiler_flags(-fsanitize=thread -ltsan)
  art_dictionary(DICTIONARY_LIBRARIES mu2e-artdaq-core_Overlays)

install_headers()
install_source()
