# ======================================================================
#  artdaq_core_mu2e main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/artdaq-core-mu2e/ups/setup_for_development
#  buildtool
# ======================================================================

cmake_minimum_required (VERSION 3.19 FATAL_ERROR)

find_package(cetmodules)

project(artdaq_core_mu2e VERSION 1.08.05)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic
 )
string(TOUPPER ${CMAKE_BUILD_TYPE} BTYPE_UC )
#if( ${BTYPE_UC} MATCHES "DEBUG" )
#  cet_add_compiler_flags(-fsanitize=address)
#endif()

#cet_report_compiler_flags()

find_package(artdaq_core 3.09.00 REQUIRED EXPORT)
find_package(mu2e_pcie_utils 2.08.00 REQUIRED EXPORT)

include(ArtDictionary)
include(BuildPlugins)

# source
add_subdirectory(artdaq-core-mu2e)

# testing
# add_subdirectory(test)

# documentation
add_subdirectory(doc)

cet_cmake_config()
