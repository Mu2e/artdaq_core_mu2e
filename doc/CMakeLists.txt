cmake_policy(VERSION 3.0.1)

include(FindDoxygen)
include(FindLATEX)
include(CetParseArgs)

SET(DOXYFILE_DIR ${CMAKE_CURRENT_LIST_DIR})

function(create_pdf_documentation)
	find_package(LATEX COMPONENTS PDFLATEX PS2PDF MAKEINDEX)
	if(LATEX_FOUND)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/${product}_API_Documentation.pdf
						   COMMAND make > pdflatex.log 2>&1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex 
					   COMMAND mv refman.pdf ${product}_API_Documentation.pdf
				   DEPENDS ${product}_doc
				   COMMENT "Generating PDF API Documentation file" VERBATIM)
			   add_custom_target(${product}_pdf ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/${product}_API_Documentation.pdf)
			   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/latex/${product}_API_Documentation.pdf DESTINATION ${product}/${version}/doc/ OPTIONAL)
	endif(LATEX_FOUND)
endfunction()


macro (create_doxygen_documentation)
if(DOXYGEN_FOUND)
	cet_parse_args( CM "EXCLUDE" "" ${ARGN})
	set(EXCLUDE_FILES "")
	if(CM_EXCLUDE)
		foreach(file ${CM_EXCLUDE})
			set(EXCLUDE_FILES "${EXCLUDE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../${file}")
		endforeach()
endif()
	configure_file(${DOXYFILE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.tex 
				  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile > doxygen.log 2>&1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} 
					  COMMENT "Generating ${project} API documentation using Doxygen" VERBATIM)
	add_custom_target(${product}_doc ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.tex)
	add_custom_command(TARGET ${product}_doc POST_BUILD
                       COMMAND echo Copying ${CMAKE_CURRENT_BINARY_DIR}/man to ${${product}_lib_dir}/../share/man
					   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/man ${${product}_lib_dir}/../share/man
					   )

    # Only build PDF documentation in MRB
	if(DEFINED ENV{MRB_BUILDDIR})
	  create_pdf_documentation()
    endif()

	# install any documentation text files
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${product}/${version} 
		PATTERN "CMakeLists.txt" EXCLUDE
		)
		
	# install doxygen-generated HTML pages and MAN pages.
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${product}/${version}/doc)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man DESTINATION ${${product}_lib_dir}/../share)
endif(DOXYGEN_FOUND)
endmacro()

#create_doxygen_documentation()
