cmake_minimum_required(VERSION 3.23...3.31)

project(fullbright)

set(CMAKE_CXX_STANDARD 23)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache CACHE PATH "")
include(vendor/CPM.cmake)

add_library(fullbright SHARED src/main.cpp)

target_compile_options(fullbright PRIVATE -Wall -Wextra -Wconversion -Wpedantic)

CPMAddPackage("gh:BasedInc/libhat@0.11.1")
target_link_libraries(fullbright PRIVATE libhat::libhat log)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(fullbright PRIVATE -flto)
    target_link_options(fullbright PRIVATE
        -flto -Wl,--version-script,${PROJECT_SOURCE_DIR}/map.txt -Wl,--no-undefined-version)
    set_target_properties(fullbright PROPERTIES
        LINK_DEPENDS ${PROJECT_SOURCE_DIR}/map.txt)
endif()

install(TARGETS fullbright LIBRARY DESTINATION mods)
