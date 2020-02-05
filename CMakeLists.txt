cmake_minimum_required(VERSION 3.12)

#@+ ========================== User configuration ===========================
set(THIS Information_Model)
#@- ===================== END OF USER CONFIGURATION =========================

project(${THIS} CXX)

enable_testing()

set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/includes")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VERBOSE_FILE_INCLUSION ON)

include(GNUInstallDirs)
include(macros)
include(conan)
conan_cmake_run(CONANFILE conanfile.txt 
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

add_subdirectory(sources)
add_subdirectory(unit_tests)