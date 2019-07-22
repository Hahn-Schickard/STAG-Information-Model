cmake_minimum_required(VERSION 3.6)

set (THIS InfoModel_Test_Suite)

file(GLOB Test_Suite "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB Test_Cases "${CMAKE_CURRENT_LIST_DIR}/*.hpp")

add_executable(${THIS} ${Test_Suite} ${Test_Cases} )

target_link_libraries(${THIS} PUBLIC
    Information_Model
    Utilities
    Model_Factory
    gtest_main)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)

if(CLANG_TIDY_EXE)               
    set_target_properties(
                        ${THIS} PROPERTIES
                        CXX_CLANG_TIDY ${CMAKE_CXX_CLANG_TIDY}
                        )
endif() 

if(WIN32)
    add_custom_command(TARGET ${THIS} POST_BUILD 
                        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                            "${CMAKE_BINARY_DIR}/bin/"
                            $<TARGET_FILE_DIR:${THIS}> 
                        COMMENT "Copying shared library files from ${CMAKE_BINARY_DIR}/bin directory to ${CMAKE_CURRENT_BINARY_DIR} directory."
                            VERBATIM) 
else(UNIX)
    add_custom_command(TARGET ${THIS} POST_BUILD
                    COMMAND ctest -C $<CONFIGURATION> --output-on-failure)
endif()