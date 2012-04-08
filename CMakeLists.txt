cmake_minimum_required (VERSION 2.6)

project(y-lpc-loader)

set( ${PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${PROJECT_NAME}_MINOR_VERSION 3 )
set( ${PROJECT_NAME}_PATCH_LEVEL 0 )
set( ${PROJECT_NAME}_VERSION
  ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})

configure_file (version.h.in ${PROJECT_BINARY_DIR}/include/version.h)

include_directories(${PROJECT_BINARY_DIR}/include)

add_definitions( -Wall )

set (LIB_YAML yaml)
set (TARG y-lpc-loader)

add_executable (y-lpc-loader y-lpc-loader.c read_config.c)
target_link_libraries (y-lpc-loader ${LIB_YAML})

set_target_properties (${TARG}
  PROPERTIES BUILD_WITH_INSTALL_RPATH ON)

install(TARGETS ${TARG} DESTINATION usr/bin)






