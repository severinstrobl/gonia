add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/catch2"
                 "third_party/catch2" EXCLUDE_FROM_ALL)

list(APPEND CMAKE_MODULE_PATH
     "${PROJECT_SOURCE_DIR}/third_party/catch2/contrib")

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
