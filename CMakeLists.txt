cmake_minimum_required(VERSION 3.26)
project(labwork5_Miraines)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(labwork5_Miraines
        .gitignore
        MyClass.cpp
        MyClass.h
        README.md
        main.cpp)

add_library(MyClass MyClass.cpp)

enable_testing()
add_subdirectory(tests)
