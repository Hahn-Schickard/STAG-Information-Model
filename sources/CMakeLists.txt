cmake_minimum_required(VERSION 3.6) 

set(THIS Information_Model_Interface)

list(APPEND ${THIS}_PUBLIC_INCLUDES_DIRS
            ${PROJECT_SOURCE_DIR}/includes/Information_Model
            
)

INCLUDE_DIRS_AND_FILES(${THIS}_PUBLIC_INCLUDES_DIRS PUBLIC_INCLUDES)

PRINT_MODULE_FILES(${THIS} "${sources_list}" "${PUBLIC_INCLUDES}")

add_library(${THIS} INTERFACE)
                         
target_include_directories(${THIS} INTERFACE
                           $<INSTALL_INTERFACE:${INCLUDES_DEST}>
                           $<INSTALL_INTERFACE:${LIBS_DEST}>)
                          
install(
    TARGETS ${THIS} 
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${LIBS_DEST}
    LIBRARY DESTINATION ${LIBS_DEST}
    COMPONENT libraries
)

install(
    FILES ${includes_list}
    DESTINATION ${INCLUDES_DEST}
    COMPONENT headers
)