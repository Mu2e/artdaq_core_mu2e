cet_make(NO_DICTIONARY 
  EXCLUDE Mu2e_fragmentNameHelper.cc
  LIBRARIES
  artdaq-core_Utilities
  DTCInterface
  ${ARTDAQ_DAQDATA}
  ${CETLIB}
  ${CETLIB_EXCEPT}
  ${MF_MESSAGELOGGER}
  )


  basic_plugin(Mu2e "fragmentNameHelper"

  artdaq-core_Data

  mu2e-artdaq-core_Overlays

  artdaq-core_Data_Artdaq_fragmentNameHelper

  MF_MessageLogger

)

cet_remove_compiler_flags(-fsanitize=address -lasan)
cet_remove_compiler_flags(-fsanitize=thread -ltsan)
  art_dictionary(DICTIONARY_LIBRARIES mu2e-artdaq-core_Overlays)

install_headers()
install_source()
