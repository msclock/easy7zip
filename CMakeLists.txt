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
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# See
# https://learn.microsoft.com/en-us/vcpkg/users/examples/overlay-triplets-linux-dynamic
set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_LIST_DIR}/cmake/custom-triplets)
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

add_subdirectory(src/_core)
