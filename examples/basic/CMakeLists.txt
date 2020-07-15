cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME basic)

add_definitions(-DUNICODE -D_UNICODE)

PROJECT(${TARGET_NAME})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_LIB_SRC)

add_executable(${TARGET_NAME} ${DIR_LIB_SRC})
add_dependencies(${TARGET_NAME} base duilib)
target_link_libraries(${TARGET_NAME} base)
target_link_libraries(${TARGET_NAME} duilib)
set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
if (MSVC)
    set_target_properties(${TARGET_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_HOME_DIRECTORY}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_HOME_DIRECTORY}/bin"
    )
endif (MSVC)
