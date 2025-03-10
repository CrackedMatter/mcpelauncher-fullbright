cmake_minimum_required(VERSION 3.15)

project(fullbright)

set(CMAKE_CXX_STANDARD 23)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache CACHE PATH "")
include(cmake/CPM.cmake)

CPMAddPackage("gh:BasedInc/libhat@0.4.0")
CPMAddPackage("gh:zyantific/zydis@4.1.1")
CPMAddPackage("gh:cursey/safetyhook@0.6.5")

add_library(fullbright SHARED src/main.cpp)

target_link_libraries(fullbright PUBLIC libhat::libhat safetyhook::safetyhook)

install(TARGETS fullbright LIBRARY DESTINATION mods)
