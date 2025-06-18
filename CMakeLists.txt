cmake_minimum_required(VERSION 3.15)

project(fullbright)

if(NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
    message(FATAL_ERROR "Unsupported ABI")
endif()

set(CMAKE_CXX_STANDARD 23)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache CACHE PATH "")
include(cmake/CPM.cmake)

add_library(fullbright SHARED src/main.cpp)

target_compile_options(fullbright PRIVATE -Wall -Wextra -Wconversion -Wpedantic)

CPMAddPackage("gh:BasedInc/libhat@0.5.0")
CPMAddPackage("gh:zyantific/zydis@4.1.1")
CPMAddPackage("gh:cursey/safetyhook@0.6.6")

target_link_libraries(fullbright PUBLIC libhat::libhat safetyhook::safetyhook)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(fullbright PRIVATE -flto)
    target_link_options(fullbright PRIVATE
            -flto -Wl,--version-script,${PROJECT_SOURCE_DIR}/map.txt -Wl,--no-undefined-version)
    set_target_properties(fullbright PROPERTIES
            LINK_DEPENDS ${PROJECT_SOURCE_DIR}/map.txt)
endif()

install(TARGETS fullbright LIBRARY DESTINATION mods)
