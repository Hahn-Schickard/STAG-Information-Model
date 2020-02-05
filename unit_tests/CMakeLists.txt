cmake_minimum_required(VERSION 3.6)

set(THIS ${PROJECT_NAME}_Test_Suite)

add_executable(${THIS})
target_sources(${THIS} PRIVATE 
                       "testRunner.cpp"
)

target_link_libraries(${THIS} PUBLIC
                              CONAN_PKG::gtest
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)

set_target_properties(${THIS} PROPERTIES
                              CXX_STANDARD 17
)