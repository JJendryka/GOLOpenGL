cmake_minimum_required(VERSION 3.9)
project(screwGL)

set(CMAKE_CXX_STANDARD 11)

add_executable(screwGL main.cpp shader.hpp shader.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

target_link_libraries(screwGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARY} glfw)