
include(ArtDictionary)

art_dictionary(
DICTIONARY_LIBRARIES
  artdaq-core-mu2e::Overlays
)

get_target_property(DictCompileOptions artdaq-core-mu2e_Overlays_root_dict COMPILE_OPTIONS)
if(DictCompileOptions)
	string(REPLACE "-fsanitize=thread" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-fsanitize=address" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-ltsan" "" DictCompileOptions "${DictCompileOptions}")
	string(REPLACE "-lasan" "" DictCompileOptions "${DictCompileOptions}")

	set_target_properties(artdaq-core-mu2e_Overlays_root_dict PROPERTIES COMPILE_OPTIONS "${DictCompileOptions}")
endif()
