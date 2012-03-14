cmake_minimum_required (VERSION 2.6)

project(lpc3250_loader)

set (TARG
  lpc3250_loader)

add_executable (lpc3250_loader lpc3250_loader.c)

set_target_properties (${TARG}
  PROPERTIES BUILD_WITH_INSTALL_RPATH ON)

install(TARGETS ${TARG} DESTINATION usr/bin)






