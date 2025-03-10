# Exemple common path, change it to your own path
# If on windows, you can use ${USER_HOME_DIRECTORY} instead of $ENV{HOME}
add_subdirectory($ENV{HOME}/Repositories/OpenGL/common ${CMAKE_BINARY_DIR}/common_build)
target_link_libraries(${PROJECT_NAME} PRIVATE common)
