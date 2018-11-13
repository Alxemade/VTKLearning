cmake_minimum_required(VERSION 2.8)

project(VTKMeanFiltering)

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})

add_executable(${PROJECT_NAME} VTKMeanFiltering.cpp)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
