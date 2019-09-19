cmake_minimum_required(VERSION 3.6)

set (THIS Information_Model_Testsuite)

file(GLOB Test_Suite "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB Test_Cases "${CMAKE_CURRENT_LIST_DIR}/*.hpp")

list(APPEND ${THIS}_PUBLIC_INCLUDES_DIRS
            ${PROJECT_SOURCE_DIR}/includes/Information_Model
)

INCLUDE_DIRS(${THIS}_PUBLIC_INCLUDES_DIRS)

add_executable(${THIS} ${Test_Suite} ${Test_Cases} ${PUBLIC_INCLUDES})

target_link_libraries(${THIS} PUBLIC
    gtest_main
    gmock_main
)

if(CLANG_TIDY_EXE)
  set_target_properties(
    ${THIS} PROPERTIES
    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
endif()       

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)

if(WIN32)
    add_custom_command(TARGET ${THIS} POST_BUILD 
                        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                            "${CMAKE_BINARY_DIR}/bin/"
                            $<TARGET_FILE_DIR:${THIS}> 
                        COMMENT "Copying shared library files from ${CMAKE_BINARY_DIR}/bin directory to ${CMAKE_CURRENT_BINARY_DIR} directory."
                            VERBATIM)
endif()
