include(FindDoxygen)
include(FindLATEX)

SET(DOXYFILE_DIR ${CMAKE_CURRENT_LIST_DIR})
function(create_pdf_documentation)
	find_package(LATEX COMPONENTS PDFLATEX PS2PDF MAKEINDEX)
	find_program(EPSTOPDF_FOUND epstopdf)

	if(LATEX_FOUND AND EPSTOPDF_FOUND)
		add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/${PROJECT_NAME}_API_Documentation.pdf
		                   COMMAND make > pdflatex.log 2>&1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex 
		                   COMMAND mv refman.pdf ${PROJECT_NAME}_API_Documentation.pdf
		                   DEPENDS ${PROJECT_NAME}_doc
		                   COMMENT "Generating ${PROJECT_NAME} PDF API Documentation file" VERBATIM)
		add_custom_target(${PROJECT_NAME}_pdf ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/${PROJECT_NAME}_API_Documentation.pdf)
		install(FILES ${CMAKE_CURRENT_BINARY_DIR}/latex/${PROJECT_NAME}_API_Documentation.pdf DESTINATION ${CMAKE_INSTALL_DATADIR}/doc OPTIONAL)
	endif(LATEX_FOUND AND EPSTOPDF_FOUND)
endfunction()


macro (create_doxygen_documentation)
if(DOXYGEN_FOUND)
    cmake_parse_arguments(CM "" "" "EXCLUDE;DEPENDS" ${ARGN})
	set(EXCLUDE_FILES "")
	if(CM_EXCLUDE)
		foreach(file ${CM_EXCLUDE})
			set(EXCLUDE_FILES "${EXCLUDE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../${file}")
		endforeach()
	endif()

	add_custom_target(${PROJECT_NAME}_doc ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tag)

	set(TAG_FILES "")

	if(CM_DEPENDS)
		foreach(dependency ${CM_DEPENDS})
			if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../${dependency})
				set(TAG_FILES "${TAG_FILES} ${CMAKE_BINARY_DIR}/${dependency}/doc/${dependency}.tag=../../${dependency}/")
				add_dependencies(${PROJECT_NAME}_doc ${dependency}_doc)
			else()
				string(TOUPPER ${dependency} dependency_uc)
				set(TAG_FILES "${TAG_FILES} $ENV{${dependency_uc}_SHARE}/doc/${dependency}.tag=$ENV{${dependency_uc}_INC}")
			endif()
		endforeach()
	endif()
	
	string(REPLACE ";" " " DOXYGEN_INCLUDE_PATH "${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}")

	configure_file(${DOXYFILE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	configure_file(${DOXYFILE_DIR}/header.html.in ${CMAKE_CURRENT_BINARY_DIR}/header.html @ONLY)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tag
	                   COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile > doxygen.log 2>&1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	                   COMMENT "Generating ${PROJECT_NAME} API documentation using Doxygen" VERBATIM)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tag DESTINATION ${CMAKE_INSTALL_DATADIR}/doc)
	create_pdf_documentation()

	# install doxygen-generated HTML pages and MAN pages.
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/ DESTINATION ${CMAKE_INSTALL_DOCDIR}/${PROJECT_NAME})
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man DESTINATION ${CMAKE_INSTALL_DATADIR})
endif(DOXYGEN_FOUND)
endmacro()

create_doxygen_documentation()
