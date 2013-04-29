if(UNIX)
	list(APPEND SOURCES host/posix/main.cpp)
endif()
list(APPEND SOURCES shadow.cpp)

add_executable(shadow ${SOURCES})
target_link_libraries(shadow)
