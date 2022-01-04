cmake_minimum_required(VERSION 3.10)

# set(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

project(examples)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(base)
add_subdirectory(duilib)
add_subdirectory(ui_components)
add_subdirectory(examples/basic)
add_subdirectory(examples/controls)
add_subdirectory(examples/datetime)
add_subdirectory(examples/layouts)
add_subdirectory(examples/move_control)
add_subdirectory(examples/richlist)
add_subdirectory(examples/virtualbox)
