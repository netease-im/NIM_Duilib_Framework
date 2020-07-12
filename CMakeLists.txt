cmake_minimum_required(VERSION 3.10)

# set(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

project(examples)

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(base)
add_subdirectory(duilib)
add_subdirectory(examples/basic)
