cmake_minimum_required(VERSION 3.12)
project(mavlink_serial_param_setter)

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
add_executable(mavlink_serial_param_setter src/mavlink_serial_param_setter.cpp)

target_link_libraries(mavlink_serial_param_setter
  ${Boost_LIBRARIES}
  pthread
)

###########
# INSTALL #
###########

# Specify installation rules
install(
  TARGETS mavlink_serial_param_setter
  EXPORT MavlinkSerialParamSetTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# Install the header files
install(DIRECTORY include/ DESTINATION include)

# Create and install CMake configuration files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MavlinkSerialParamSet/MavlinkSerialParamSetConfigVersion.cmake"
  VERSION 1.0.0
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/MavlinkSerialParamSetConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/MavlinkSerialParamSetConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_CONFIG_INSTALL_DESTINATION}"
)

export(
  TARGETS mavlink_serial_param_setter
  NAMESPACE MavlinkSerialParamSet::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/MavlinkSerialParamSet/MavlinkSerialParamSetTargets.cmake"
)

install(
  EXPORT MavlinkSerialParamSetTargets
  FILE MavlinkSerialParamSetTargets.cmake
  NAMESPACE MavlinkSerialParamSet::
  DESTINATION "${CMAKE_CONFIG_INSTALL_DESTINATION}"
)

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/MavlinkSerialParamSet/MavlinkSerialParamSetConfigVersion.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/MavlinkSerialParamSetConfig.cmake"
  DESTINATION "${CMAKE_CONFIG_INSTALL_DESTINATION}"
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
  COMMAND ${CMAKE_COMMAND} -E echo "Removing directory ${CMAKE_INSTALL_PREFIX}/${CMAKE_CONFIG_INSTALL_DESTINATION}"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_INSTALL_PREFIX}/${CMAKE_CONFIG_INSTALL_DESTINATION}"
)

#############
# Packaging #
#############

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nikola Popara")
set(CPACK_PACKAGE_DESCRIPTION "A MAVLink SERIAL param setter.")
set(CPACK_PACKAGE_CONTACT "nikola.popara@airvolute.com")

include(CPack)