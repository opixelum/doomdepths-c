cmake_minimum_required(VERSION 3.18)
project(doomdepths-c VERSION 0.1.0 LANGUAGES C)
find_package(SQLite3 REQUIRED)
file(GLOB SOURCES  "src/*.c")
add_executable(doomdepths-c ${SOURCES})
target_include_directories(doomdepths-c PRIVATE include)
target_link_libraries(doomdepths-c PRIVATE ${SQLite3_LIBRARIES})
