cet_make(NO_DICTIONARY LIBRARIES
  artdaq-core_Utilities
  ${ARTDAQ_DAQDATA}
  ${CETLIB}
  ${CETLIB_EXCEPT}
  )

cet_remove_compiler_flags(-fsanitize=address -lasan)
cet_remove_compiler_flags(-fsanitize=thread -ltsan)
  art_dictionary(DICTIONARY_LIBRARIES mu2e-artdaq-core_Overlays)

install_headers()
install_source()
