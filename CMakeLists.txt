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
        Easy/PCH/*.hpp
)

file(GLOB_RECURSE OPENGL_SOURCES
        Easy/Platform/OpenGLEngineContext.cpp
)

file(GLOB_RECURSE OPENGL_MODULES
        Easy/Platform/OpenGLEngineContext.ixx
)

file(GLOB ImGuiGLFW_Sources
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

add_library(${PROJECT_NAME}
        ${VendorFiles}
        ${Core_MODULES}
        ${Core_SOURCES}
        ${OPENGL_SOURCES}
        ${OPENGL_MODULES}
        ${ImGuiGLFW_Sources}
)

target_compile_options(${PROJECT_NAME}
        PRIVATE
        -pedantic
        -Wall
        -Wextra
        -Wsign-conversion
        # integer conversion warnings
        -Wconversion
        -Werror
        -Wno-macro-redefined
        -Wno-deprecated-declarations
)

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        ${Core_MODULES}
        ${OPENGL_MODULES}
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
        imgui
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        std_module
        glfw
)