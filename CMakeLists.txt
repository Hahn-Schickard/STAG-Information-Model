cmake_minimum_required(VERSION 3.6)

set(THIS Project_Template)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_POSITION_INDEPENDENT_CODE on)
set(VERBOSE_FILE_INCLUSION on)
set(PROJECT_FLAGS C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

project(${THIS} ${PROJECT_FLAGS})

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(macros)
include(compiler_settings)

if(UNIX)
    include(linter)
endif()

include(dependencies)

set(includes_dir "${PROJECT_SOURCE_DIR}/includes")
set(clang_output "${PROJECT_SOURCE_DIR}/tools/clang-tidy-suggested-fixes")

include_directories(${includes_dir})
   
add_subdirectory(sources)
add_subdirectory(tests/unit_tests/InfoModel)
