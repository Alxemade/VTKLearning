cmake_minimum_required(VERSION 2.8)

project(vtkImageReaderDemo2)

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})

add_executable(${PROJECT_NAME} vtkImageReaderDemo2.cpp)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
