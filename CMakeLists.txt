cmake_minimum_required(VERSION 3.18)
project(DoomdepthsC VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)

file(GLOB SOURCES  "src/*.c")

include(CTest)
enable_testing()

add_executable(DoomdepthsC ${SOURCES})

target_include_directories(DoomdepthsC PRIVATE include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
