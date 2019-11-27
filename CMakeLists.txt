cmake_minimum_required(VERSION 3.14)
project(crisp)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-ftemplate-depth-5000)

FILE(GLOB Source include/interpreter/*
        include/util/*)
set(crisp ${Source})

include_directories(include)

# Add executables for files in "examples/"
FILE(GLOB Examples examples/*)

FOREACH (path ${Examples})
    get_filename_component(name ${path} NAME_WE)
    add_executable(${name}.exe ${path} ${crisp})
ENDFOREACH ()

# Add executables for files in "test/"
FILE(GLOB Tests test/*)

FOREACH (path ${Tests})
    get_filename_component(name ${path} NAME_WE)
    add_executable(${name}.exe ${path} ${crisp})
ENDFOREACH ()

FILE(GLOB Demos demos/*)

FOREACH (path ${Demos})
    get_filename_component(name ${path} NAME_WE)
    add_executable(${name}.exe ${path} ${crisp})
ENDFOREACH ()