cmake_minimum_required(VERSION 3.10)

add_definitions(-DUNICODE -D_UNICODE)

PROJECT(basic)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_LIB_SRC)

add_executable(basic ${DIR_LIB_SRC})
add_dependencies(basic base duilib)
target_link_libraries(basic base)
target_link_libraries(basic duilib)
set_target_properties(basic PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

add_custom_command(TARGET basic POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
	${PROJECT_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/basic.exe 
	${CMAKE_HOME_DIRECTORY}/bin/basic.exe)
