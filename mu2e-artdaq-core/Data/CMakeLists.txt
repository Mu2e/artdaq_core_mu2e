cet_make(
NO_DICTIONARY
  )

cet_remove_compiler_flags(-fsanitize=address -lasan)
cet_remove_compiler_flags(-fsanitize=thread -ltsan)
  art_dictionary(DICTIONARY_LIBRARIES artdaq-core_Data)
