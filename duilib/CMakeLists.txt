include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_LIB_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/Animation ANIMATION_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/Box BOX_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/Control CONTROL_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/Core CORE_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/Render RENDER_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/Utils UTILS_SRC)

add_library(duilib ${DIR_LIB_SRC} ${ANIMATION_SRC} ${BOX_SRC} ${CONTROL_SRC} ${CORE_SRC} ${RENDER_SRC} ${UTILS_SRC})
target_link_libraries(duilib comctl32)
target_link_libraries(duilib gdiplus)
target_link_libraries(duilib shlwapi)
target_link_libraries(duilib imm32)
target_link_libraries(duilib Msimg32)

