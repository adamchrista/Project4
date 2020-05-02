cmake_minimum_required(VERSION 3.15)
project(Project4)

set(CMAKE_CXX_STANDARD 14)

set(input01 "munkresinput.txt")

foreach(file IN LISTS input01)
    configure_file(${file} ${file} COPYONLY)
endforeach()

add_executable(Project4 main.cpp RowOrColumn.h Value.h Matrix.h munkresinput.txt)