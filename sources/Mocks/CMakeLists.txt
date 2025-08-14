
#@+ ======================== User TARGET NAME configuration =============================
set(THIS Mocks)

list(APPEND THIS_INCLUDES_DIRS
    ${INCLUDES_DIR}/Mocks
)
#@- =========================== END OF USER CONFIGURATION ===============================

set(TARGET ${PROJECT_NAME}_${THIS})

INCLUDE_DIRS_AND_FILES("${THIS_INCLUDES_DIRS}" THIS_INCLUDES)

add_library(${TARGET} INTERFACE)

#@+ ======================== User DEPENDENCIES configuration ============================
target_link_libraries(${TARGET}
    INTERFACE
        ${PROJECT_NAME}_Interface
        GTest::gmock
)
#@- =========================== END OF USER CONFIGURATION ===============================

target_include_directories(${TARGET}
    INTERFACE
        $<BUILD_INTERFACE:${INCLUDES_DIR}/Mocks>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

target_compile_features(${TARGET} INTERFACE cxx_std_17)

PRINT_TARGET_PROPERTIES(${TARGET})

INSTALL_TARGET(${TARGET})
INSTALL_HEADERS(HEADERS "${THIS_INCLUDES}")
