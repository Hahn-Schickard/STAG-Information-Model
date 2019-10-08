cmake_minimum_required(VERSION 3.6) 

set(THIS Information_Model_Interface)

list(APPEND PUBLIC_INCLUDES_DIRS
            ${INCLUDES_DIR}/Information_Model
)

INCLUDE_DIRS_AND_FILES("${PUBLIC_INCLUDES_DIRS}" PUBLIC_INCLUDES)

add_library(${THIS} INTERFACE)
                         
target_include_directories(${THIS} INTERFACE
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/Information_Model>
                           $<INSTALL_INTERFACE:${THIS_PACKAGE_NAME}/${INCLUDES_DEST}/Information_Model>
)

install(
    TARGETS ${THIS}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY ${PUBLIC_INCLUDES_DIRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
