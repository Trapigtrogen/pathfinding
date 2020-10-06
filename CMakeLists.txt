cmake_minimum_required (VERSION 2.6)

# set the project name
project(Pathfinding)

if (WIN32)
    # Specify directories in which the linker will look for libraries.
    link_directories("../../glut")
endif()

set(CMAKE_VERBOSE_MAKEFILE on)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} 
                    ${GLUT_INCLUDE_DIRS}
                    )

# add the executable
add_executable(Pathfinding path_finding/pathfinding.cpp)

target_include_directories(Pathfinding PUBLIC
                           "${PROJECT_BINARY_DIR}"
						   )
						   
target_link_libraries(Pathfinding ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

configure_file(path_finding/input.bmp ./input.bmp COPYONLY)