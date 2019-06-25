cmake_minimum_required(VERSION 2.8.2)

project(google-download NONE)

include(ExternalProject)

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    SOURCE_DIR "C:/projects/GatewayRedesign/Fork/information-modeller/tests/googletest-src"
    BINARY_DIR "C:/projects/GatewayRedesign/Fork/information-modeller/build/googletest-build"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
