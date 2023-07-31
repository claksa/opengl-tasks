cmake_minimum_required(VERSION 3.24)
project(graphics)

set(GLAD_SRC external/glad/src)
set(SAMPLES src/samples)


add_executable(${PROJECT_NAME} ${GLAD_SRC}/glad.c utils.hpp main.cpp)

add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME}
        PUBLIC external/glfw/include
        PUBLIC external/glad/include
        )

target_link_directories(${PROJECT_NAME}
        PRIVATE external/glfw/src
        PRIVATE external/glad/src
        )

target_link_libraries(${PROJECT_NAME} glfw3 ${CMAKE_DL_LIBS})
