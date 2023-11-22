cmake_minimum_required (VERSION 3.4)
project(Pathfinding)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BITNESS 64)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external/Drawpp/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Pathfinding ${SOURCES})
target_link_libraries(Pathfinding ${CMAKE_SOURCE_DIR}/external/Drawpp/libDrawpp.a)

file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}/bin")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
