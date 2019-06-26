cmake_minimum_required(VERSION 3.6)

set(THIS Project_Template)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_POSITION_INDEPENDENT_CODE on)
set(VERBOSE_FILE_INCLUSION on)
set(PROJECT_FLAGS C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

project(${THIS} ${PROJECT_FLAGS})

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FORCE_WINDOWS_LINTING OFF CACHE BOOL "Force linting with clang-tidy on Windows")
set(CLANG_OUTPUT "${PROJECT_SOURCE_DIR}/clang-tidy-suggested-fixes.yml")
set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/includes")

include(macros)
include(compiler_settings)
include(linter)

include(dependencies)

include_directories(${INCLUDES_DIR})
   
add_subdirectory(sources)
add_subdirectory(tests/unit_tests/InfoModel)