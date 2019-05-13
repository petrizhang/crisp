cmake_minimum_required(VERSION 3.14)
project(tispp)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-ftemplate-depth-5000)

set(tispp src/crisp.hpp)

add_executable(test.exe tests/test.cpp ${tispp})

include_directories(src)

# Add executables for files in "examples/"
FILE(GLOB Examples examples/*)

FOREACH (path ${Examples})
    get_filename_component(name ${path} NAME_WE)
    add_executable(${name}.exe ${path})
ENDFOREACH ()