cmake_minimum_required(VERSION 3.24)
project(graphics)

add_executable(${PROJECT_NAME} main.cpp external/glad/src/glad.c)

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
