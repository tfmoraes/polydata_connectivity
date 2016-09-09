cmake_minimum_required(VERSION 2.8)

PROJECT(test_connectivity)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenGL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )

add_library(connectivity SHARED connectivity.cpp)

add_executable(test_connectivity MACOSX_BUNDLE test_connectivity connectivity)

if(VTK_LIBRARIES)
  target_link_libraries(test_connectivity ${VTK_LIBRARIES} ${OPENGL_LIBRARIES})
else()
  target_link_libraries(test_connectivity vtkHybrid)
endif()


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
