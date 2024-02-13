cmake_minimum_required(VERSION 3.12)
project(mavlink_sys_id_set VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIG_INSTALL_DESTINATION lib/cmake/MavlinkSerialParamSet)

# find dependencies
find_package(Boost REQUIRED COMPONENTS system container)
find_path(MAVLINK_INCLUDE_DIR NAMES mavlink/common/mavlink.h)

# Header files locations
include_directories(
  ${Boost_INCLUDE_DIRS}
  ${MAVLINK_INCLUDE_DIR}
  include
)

#########
# BUILD #
#########

# Executable
add_executable(mavlink_sys_id_set src/mavlink_sys_id_set.cpp)

target_link_libraries(mavlink_sys_id_set
  ${Boost_LIBRARIES}
  pthread
)

###########
# INSTALL #
###########

# Specify installation rules
install(
  TARGETS mavlink_sys_id_set
  RUNTIME DESTINATION bin
)

#############
# UNINSTALL #
#############
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND ${CMAKE_COMMAND} -E echo "Removing files defined in install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!
)

#############
# Packaging #
#############

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nikola Popara")
set(CPACK_PACKAGE_DESCRIPTION "A MAVLink SERIAL SYS ID setter.")
set(CPACK_PACKAGE_CONTACT "nikola.popara@airvolute.com")

include(CPack)