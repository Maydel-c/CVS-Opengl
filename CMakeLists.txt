cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(
  cvs
  VERSION 1.0
  DESCRIPTION "CVS: Definitive Edition"
  LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_subdirectory(glm)

add_library(glad STATIC ./glad/src/glad.c)

target_include_directories(glad PUBLIC ./glad/include)

add_executable(cvs main.cpp)

target_link_libraries(cvs PUBLIC OpenGL::GL glm::glm glfw glad)
