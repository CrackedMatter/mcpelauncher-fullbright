cmake_minimum_required(VERSION 3.15)

project(fullbright)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(libhat
        GIT_REPOSITORY https://github.com/BasedInc/libhat.git
        GIT_TAG e457476d3c4f38242ec65998007221a413f1f26f
        EXCLUDE_FROM_ALL)

FetchContent_Declare(safetyhook
        GIT_REPOSITORY https://github.com/cursey/safetyhook.git
        GIT_TAG b046e123dc69821f2c375161e0adef3c6d9c9db4
        EXCLUDE_FROM_ALL)

set(SAFETYHOOK_FETCH_ZYDIS ON)

FetchContent_MakeAvailable(libhat safetyhook)

add_library(fullbright SHARED src/main.cpp)

target_link_libraries(fullbright PUBLIC libhat safetyhook::safetyhook)

install(TARGETS fullbright LIBRARY DESTINATION mods)
