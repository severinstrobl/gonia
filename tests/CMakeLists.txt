# Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
#
#  SPDX-License-Identifier: MIT

include(FetchContent)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

set(unit_tests TestPrimitives TestIntersect TestIsSimple)

foreach(unit_test ${unit_tests})
  add_executable(${unit_test} ${unit_test}.cpp)
  target_compile_options(${unit_test} PRIVATE -pedantic -Wall -Wextra -Wshadow
                                              -Werror)
  target_compile_definitions(${unit_test} PRIVATE CATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
  target_link_libraries(${unit_test} Catch2::Catch2WithMain coverage)
  catch_discover_tests(
    ${unit_test}
    REPORTER
    junit
    OUTPUT_DIR
    ${CMAKE_BINARY_DIR}/
    OUTPUT_PREFIX
    catch2_testing_
    OUTPUT_SUFFIX
    .xml)
endforeach()
