# project name
project(mu2e-artdaq-core_BuildInfo)

# the utcDateTime variable is used by the GetPackageBuildInfo code
# to report the build datetime
execute_process( COMMAND "date" "-u" "+%d-%b-%Y %H:%M:%S %Z" OUTPUT_VARIABLE rawUTCDate )
string( STRIP ${rawUTCDate} utcDateTime )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/GetPackageBuildInfo.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/GetPackageBuildInfo.cc @ONLY )

art_make_library( LIBRARY_NAME artdaq-core-demo_BuildInfo
                  SOURCE
		  ${CMAKE_CURRENT_BINARY_DIR}/GetPackageBuildInfo.cc
                 )

install_headers()
install_source()
