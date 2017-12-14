include(FindDoxygen)
include(FindLATEX)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
    #message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

if(DOXYGEN_FOUND)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.tex 
				  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile > doxygen.log 2>&1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
					  COMMENT "Generating ${project} API documentation using Doxygen" VERBATIM)
	add_custom_target(${product}_doc ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.tex)
	add_custom_command(TARGET ${product}_doc POST_BUILD
                       COMMAND echo Copying ${CMAKE_CURRENT_BINARY_DIR}/man to ${LIBRARY_OUTPUT_PATH}/../share/man
					   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/man ${LIBRARY_OUTPUT_PATH}/../share/man
					   )
	find_package(LATEX COMPONENTS PDFLATEX PS2PDF MAKEINDEX)
	if(LATEX_FOUND)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/API_Documentation.pdf
						   COMMAND make > pdflatex.log 2>&1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex 
					   COMMAND mv refman.pdf API_Documentation.pdf
				   DEPENDS ${product}_doc
				   COMMENT "Generating PDF API Documentation file" VERBATIM)
			   # ALL removed from this add_custom_target, as the PDF does not build correctly on woof
			   add_custom_target(${product}_pdf DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/API_Documentation.pdf)
			   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/latex/API_Documentation.pdf DESTINATION ${product}/${version}/doc/ OPTIONAL)
	endif(LATEX_FOUND)
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${product}/${version} 
		PATTERN "CMakeLists.txt" EXCLUDE
		PATTERN "Doxyfile.in" EXCLUDE
		)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${product}/${version}/doc)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man DESTINATION ${${product}_lib_dir}/../share)
endif(DOXYGEN_FOUND)
