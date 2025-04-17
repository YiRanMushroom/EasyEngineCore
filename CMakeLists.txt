cmake_minimum_required(VERSION 3.30)

project(EasyEngineCore)

set(PROJECT_NAME EasyEngineCore)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE Core_MODULES
        Easy/Core/*.ixx
        Easy/Core/*.cppm
)

file(GLOB_RECURSE Core_SOURCES
        Easy/Core/*.cpp
)

file(GLOB VendorFiles glad/src/glad.c)

file(GLOB_RECURSE PreCompiledHeaders
        Easy/PCH/OpenGL.pch
)

add_library(${PROJECT_NAME}
        ${VendorFiles}
        ${Core_MODULES}
        ${Core_SOURCES}
)

target_compile_options(${PROJECT_NAME}
        PRIVATE
        -pedantic
        -Wall
        -Wextra
        -Wsign-conversion
        -Wdouble-promotion
        -Werror
)

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        ${Core_MODULES}
)

add_subdirectory(std_module)

add_subdirectory(glfw)

target_precompile_headers(${PROJECT_NAME}
        PUBLIC
        ${PreCompiledHeaders}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        Easy/PCH
        glad/include
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        std_module
        glfw
)