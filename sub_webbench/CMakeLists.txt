# cmake_minimum_required(VERSION 3.10.2)

# project(sub_webbench)
aux_source_directory(. SRC_DIR)
add_library(CoreFunctions ${SRC_DIR})
