cet_make(NO_DICTIONARY 
  EXCLUDE Mu2e_fragmentNameHelper.cc
  LIBRARIES
  artdaq_core::artdaq-core_Utilities
  pcie_linux_kernel_module::DTCInterface
  cetlib::cetlib
  cetlib_except::cetlib_except
  messagefacility::MF_MessageLogger
  )


  basic_plugin(Mu2e "fragmentNameHelper"
    artdaq_core::artdaq-core_Data
    mu2e-artdaq-core_Overlays
    artdaq_core::artdaq-core_Data_Artdaq_fragmentNameHelper
    messagefacility::MF_MessageLogger
 )

cet_remove_compiler_flags(-fsanitize=address -lasan)
cet_remove_compiler_flags(-fsanitize=thread -ltsan)
  art_dictionary(DICTIONARY_LIBRARIES mu2e-artdaq-core_Overlays)

install_headers()
install_source()
