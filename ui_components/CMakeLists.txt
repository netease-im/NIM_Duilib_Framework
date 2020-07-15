include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_LIB_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/comboex COMBOEX_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/menu MENU_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/modal_wnd MODAL_WND_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/msgbox MSGBOX_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/shadow_wnd SHADOW_WND_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/toast TOAST_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/windows_manager WINDOWS_MANAGER_SRC)

add_library(ui_components 
	${DIR_LIB_SRC} 
	${COMBOEX_SRC} 
	${MENU_SRC} 
	${MODAL_WND_SRC} 
	${MSGBOX_SRC} 
	${SHADOW_WND_SRC} 
	${TOAST_SRC} 
	${WINDOWS_MANAGER_SRC}
)

target_link_libraries(${TARGET_NAME} base)
target_link_libraries(${TARGET_NAME} duilib)
