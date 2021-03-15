project(base)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/encrypt ENCRYPT_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/file FILE_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/framework FRAMEWORK_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/hardware HARDWARE_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/memory MEMORY_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/network NETWORK_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/synchronization SYNCHRONIZATION_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/third_party/convert_utf CONVERT_UTF)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/thread THREAD_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/time TIME_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/util UTIL_SRC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/win32 WIN32_SRC)

add_library(base 
	${ENCRYPT_SRC} 
	${FILE_SRC} 
	${FRAMEWORK_SRC} 
	${HARDWARE_SRC} 
	${MEMORY_SRC} 
	${NETWORK_SRC} 
	${SYNCHRONIZATION_SRC} 
	${CONVERT_UTF} 
	${THREAD_SRC} 
	${TIME_SRC} 
	${UTIL_SRC} 
	${WIN32_SRC}
)
