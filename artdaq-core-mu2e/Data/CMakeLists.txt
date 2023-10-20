cet_make_library(
SOURCE 
EventHeader.cc
RunHeader.cc
SubRunHeader.cc
TimeStamp.cc
Mu2eEventHeader.cc
DTCDataDecoder.cc 
CalorimeterDataDecoder.cc
CalorimeterDataDecoderNew.cc
CRVDataDecoder.cc 
TrackerDataDecoder.cc
LIBRARIES PUBLIC
  artdaq_core_mu2e::artdaq-core-mu2e_Overlays
  )

include(ArtDictionary)

art_dictionary(DICTIONARY_LIBRARIES
  artdaq_core_mu2e::artdaq-core-mu2e_Data
)

get_target_property(DictCompileOptions artdaq-core-mu2e_Data_dict COMPILE_OPTIONS)
if(DictCompileOptions)
	string(REPLACE "-fsanitize=thread" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-fsanitize=address" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-ltsan" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-lasan" "" DictCompileOptions "${DictCompileOptions}")

	set_target_properties(artdaq-core-mu2e_Data_dict PROPERTIES COMPILE_OPTIONS "${DictCompileOptions}")
endif()

install_headers()
install_source()
