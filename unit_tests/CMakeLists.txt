cmake_minimum_required(VERSION 3.6)

set(THIS Unit_Tests_Runner)

file(GLOB Mock_Test_Suite "${CMAKE_CURRENT_LIST_DIR}/Mock_Tests/*")

find_package(GTest REQUIRED)

add_executable(${THIS})
target_sources(${THIS} PRIVATE 
                       "testRunner.cpp"
                       ${Mock_Test_Suite}
)

target_link_libraries(${THIS} PUBLIC
                              GTest::GTest
                              Model
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)

set_target_properties(${THIS} PROPERTIES
                              CXX_STANDARD 17
)