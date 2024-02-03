cmake_minimum_required(VERSION 3.15...3.26)

# Vcpkg build environment
if(DEFINED ENV{VCPKG_ROOT})
  message(STATUS "VCPKG_ROOT:$ENV{VCPKG_ROOT}")
  set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
else()
  message(
    FATAL_ERROR "Missing VCPKG_ROOT, please check the existence of VCPKG_ROOT")
endif()

# set cmake tool chain
file(TO_CMAKE_PATH ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
     vcpkg_toolchain_file)
set(CMAKE_TOOLCHAIN_FILE ${vcpkg_toolchain_file})

# See
# https://learn.microsoft.com/en-us/vcpkg/users/examples/overlay-triplets-linux-dynamic
set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_LIST_DIR}/cmake/triplets)
set(VCPKG_OVERLAY_PORTS ${CMAKE_CURRENT_LIST_DIR}/cmake/ports)
set(VCPKG_VERBOSE
    ON
    CACHE BOOL "Vcpkg VCPKG_VERBOSE")

if(NOT DEFINED SKBUILD_PROJECT_NAME)
  set(SKBUILD_PROJECT_NAME _core)
  set(SKBUILD_PROJECT_VERSION 0.0.0)
endif()

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

# Shared cmake modules from cmake registry, see
# https://github.com/msclock/cmake-registry.
find_package(cmake-modules REQUIRED)
include(cmake-modules/configure/Default)
include(cmake-modules/configure/Common)
include(cmake-modules/configure/CheckBuildDir)
include(cmake-modules/configure/UniqueOutputBinaryDir)
include(cmake-modules/build/Default)
include(cmake-modules/build/Ccache)
include(cmake-modules/build/LinkOptimization)
include(cmake-modules/test/Default)
include(cmake-modules/test/GoogleTest)
include(cmake-modules/install/Common)
include(cmake-modules/install/Default)
include(cmake-modules/install/InstallDependency)
include(cmake-modules/install/Runpath)

# Add _DEBUG macro
add_debug_macro()

create_uninstall_target()

# Project custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ConfigureVersion)

add_subdirectory(src/smooth7zip)
add_subdirectory(src/_core)

# Print project final information
show_project_version()
show_vcpkg_configuration()
show_installation()
