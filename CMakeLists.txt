#1.cmake verson，指定cmake版本 
cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

message(${CMAKE_SOURCE_DIR})

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(simple_web_bench)
# SET(LIBRARY_OUTPUT_DIRECTORY ./lib)
include_directories(./ ./sub_webbench)
add_subdirectory(sub_webbench)

add_executable(webbench ./webbench.cpp)
target_link_libraries(webbench CoreFunctions)