cmake_minimum_required(VERSION 2.8.2)

project(json-download NONE)

include(ExternalProject)

ExternalProject_Add(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG master
    SOURCE_DIR "C:/projects/GatewayRedesign/Fork/information-modeller/libs/json-src"
    BINARY_DIR ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
