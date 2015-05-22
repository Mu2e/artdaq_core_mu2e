# ======================================================================
#  mu2e_artdaq-core main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/mu2e_artdaq-core/ups/setup_for_development
#  buildtool
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH 
        $ENV{ART_DIR}/Modules
        $ENV{CETBUILDTOOLS_DIR}/Modules
        ${CMAKE_MODULE_PATH})


include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic
 EXTRA_CXX_FLAGS -D_GLIBCXX_USE_NANOSLEEP
 )

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( cetbuildtools v4_01_04 )
find_ups_product( artdaq_core v1_03_04 )
find_ups_product( art v1_10_00 )
#find_ups_product( fhiclcpp v2_16_00 )
#find_ups_product( messagefacility v1_10_00 )
find_ups_product( TRACE v3_03_00 )
find_ups_product( cpp0x v1_03_00 )
find_ups_product( cetlib v1_03_00 )
#find_ups_root( v5_34_00 )
find_ups_boost( v1_50_0 filesystem
                        program_options
                        unit_test_framework
 )

# have to make sure cmake doesn't select the system libraries
# define libsigc
cet_find_library( SIGC    NAMES sigc-2.0  PATHS ENV LIBSIGCPP_LIB NO_DEFAULT_PATH )

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)
include(CetTest)

# source
add_subdirectory(mu2e-artdaq-core)

# testing
# add_subdirectory(test)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
