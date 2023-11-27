# Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
#
#  SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.16)
project(gonia)

enable_language(CXX)

string(TOUPPER ${CMAKE_PROJECT_NAME}_ option_prefix)
string(REPLACE - _ option_prefix ${option_prefix})

option(${option_prefix}TESTS "Build the tests" on)
option(${option_prefix}COVERAGE "Build with coverage support" off)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(coverage INTERFACE)

if(${option_prefix}COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
  if(NOT CMAKE_BUILD_TYPE MATCHES "(Debug|RelWithDebInfo)")
    message(
      FATAL_ERROR
        "Generation of coverage information requires debug information, set "
        "CMAKE_BUILD_TYPE=Debug or CMAKE_BUILD_TYPE=RelWithDebInfo.")
  endif()

  message(STATUS "Enabling generation of coverage reports")
  set(coverage_compile_flags "--coverage")

  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-fprofile-abs-path" have_fprofile_abs_path)
  if(have_fprofile_abs_path)
    list(APPEND coverage_compile_flags "-fprofile-abs-path")
  endif()

  target_compile_options(coverage INTERFACE ${coverage_compile_flags})
  target_link_options(coverage INTERFACE "--coverage")
endif()

if(${option_prefix}TESTS)
  enable_testing()
  message(STATUS "Enabling building of tests")
  add_subdirectory(tests)
endif()
