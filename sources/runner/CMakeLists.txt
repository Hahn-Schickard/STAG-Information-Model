cmake_minimum_required(VERSION 3.6) 

set(THIS Runner_MAIN) # Change the name of THIS variable to your module name

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB includes_list  "${PROJECT_SOURCE_DIR}/includes/utils/*.hpp")

PRINT_MODULE_FILES(${THIS} "${sources_list}" "${includes_list}")

# ==============================================================================================================================
# =============================================== YOUR CONFIGURATION GOES HERE =================================================
# ==============================================================================================================================
# =============================================== EXAMPLE SUBMODULE INCLUSION===================================================
# ================================================= REMOVED UNUSED PARTS! ======================================================

add_executable(${THIS} 
               ${sources_list}
               ${includes_list})

target_link_libraries(${THIS} PUBLIC 
               Print_Text_Lib
               InfoModel_Lib)
   
# ==============================================================================================================================
# ================================================ END OF YOUR CONFIGURATION ===================================================

add_custom_command(TARGET ${THIS} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E make_directory 
                        ${CMAKE_BINARY_DIR}/bin
                    COMMAND ${CMAKE_COMMAND} -E copy 
                        $<TARGET_FILE:${THIS}> 
                        "${CMAKE_BINARY_DIR}/bin"
                    COMMENT "Copying shared library files from ${CMAKE_CURRENT_BINARY_DIR} directory to ${CMAKE_BINARY_DIR}/bin directory."
                        VERBATIM)