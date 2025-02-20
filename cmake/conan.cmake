cmake_minimum_required(VERSION 3.24)

if (WIN32)
  set(CMAKE_OBJECT_PATH_MAX 32767)
endif ()

include("${CMAKE_CURRENT_LIST_DIR}/conan_provider.cmake")

cmake_language(
  SET_DEPENDENCY_PROVIDER conan_provide_dependency
  SUPPORTED_METHODS FIND_PACKAGE
)
