cmake_minimum_required (VERSION 2.6)

project(y-lpc-loader)

add_definitions( -Wall )

set (LIB_YAML yaml)
set (TARG y-lpc-loader)

add_executable (y-lpc-loader y-lpc-loader.c read_config.c)
target_link_libraries (y-lpc-loader ${LIB_YAML})

set_target_properties (${TARG}
  PROPERTIES BUILD_WITH_INSTALL_RPATH ON)

install(TARGETS ${TARG} DESTINATION usr/bin)






