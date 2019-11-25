cmake_minimum_required(VERSION 3.14)
project(crisp)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-ftemplate-depth-5000)

FILE(GLOB Source include/interpreter/*
        include/util/*)
set(crisp ${Source})

add_executable(test.exe tests/test.cpp ${crisp})

include_directories(include)

# Add executables for files in "examples/"
FILE(GLOB Examples examples/*)

FOREACH (path ${Examples})
    get_filename_component(name ${path} NAME_WE)
    add_executable(${name}.exe ${path})
ENDFOREACH ()