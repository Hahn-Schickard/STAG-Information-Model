cmake_minimum_required(VERSION 3.6) 

set(THIS Information_Model_Interface)

list(APPEND PUBLIC_INCLUDES_DIRS
            ${PROJECT_SOURCE_DIR}/includes/Information_Model)

INCLUDE_DIRS_AND_FILES(PUBLIC_INCLUDES_DIRS PUBLIC_INCLUDES)
PRINT_MODULE_FILES(${THIS} "${sources_list}" "${PUBLIC_INCLUDES}")

add_library(${THIS} INTERFACE)
                         
target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/Information_Model>
                           $<INSTALL_INTERFACE:${THIS_PACKAGE_NAME}/${INCLUDES_DEST}/Information_Model>)
                          
install(
    TARGETS ${THIS} 
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${LIBS_DEST}
    LIBRARY DESTINATION ${LIBS_DEST}
    COMPONENT libraries
)

install(
    DIRECTORY ${PUBLIC_INCLUDES_DIRS}
    DESTINATION ${INCLUDES_DEST}
    COMPONENT headers
)