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

# Events
file(GLOB_RECURSE Event_MODULES
        Easy/Events/*.ixx
        Easy/Events/*.cppm
)

file(GLOB_RECURSE Event_SOURCES
        Easy/Events/*.cpp
)

# ImGui
file(GLOB_RECURSE ImGui_MODULES
        Easy/ImGui/*.ixx
        Easy/ImGui/*.cppm
)
file(GLOB_RECURSE ImGui_SOURCES
        Easy/ImGui/*.cpp
)

# Platform
file(GLOB_RECURSE Platform_MODULES
        Easy/Platform/*.ixx
        Easy/Platform/*.cppm
)
file(GLOB_RECURSE Platform_SOURCES
        Easy/Platform/*.cpp
)

# Renderer
file(GLOB_RECURSE Renderer_MODULES
        Easy/Renderer/*.ixx
        Easy/Renderer/*.cppm
)
file(GLOB_RECURSE Renderer_SOURCES
        Easy/Renderer/*.cpp
)

# Vendor
file(GLOB_RECURSE Vendor_MODULES
        Easy/Vendor/*.ixx
        Easy/Vendor/*.cppm
)
file(GLOB_RECURSE Vendor_SOURCES
        Easy/Vendor/*.cpp
)

file(GLOB VendorFiles vendor/glad/src/glad.c)

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
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(${PROJECT_NAME}
        ${VendorFiles}
        ${Core_MODULES}
        ${Core_SOURCES}
        ${Event_MODULES}
        ${Event_SOURCES}
        ${ImGui_MODULES}
        ${ImGui_SOURCES}
        ${OPENGL_SOURCES}
        ${OPENGL_MODULES}
        ${Platform_MODULES}
        ${Platform_SOURCES}
        ${Renderer_MODULES}
        ${Renderer_SOURCES}
        ${Vendor_MODULES}
        ${Vendor_SOURCES}
        ${ImGuiGLFW_Sources}
)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        "EZ_DEBUG=1"
        "EZ_PLATFORM_WINDOWS"
)

target_compile_options(${PROJECT_NAME}
        PRIVATE
        #        -pedantic
        #        -Wall
        #        -Wextra
        #        -Wno-sign-conversion
        #        -Wconversion
        -Wno-macro-redefined
        -Wno-deprecated-declarations
        #        -Werror
)

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        ${Core_MODULES}
        ${Event_MODULES}
        ${ImGui_MODULES}
        ${OPENGL_MODULES}
        ${Platform_MODULES}
        ${Renderer_MODULES}
        ${Vendor_MODULES}
)

add_subdirectory(vendor/std_module)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)

target_precompile_headers(${PROJECT_NAME}
        PUBLIC
        ${PreCompiledHeaders}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        Easy/PCH
        Easy
        vendor/glad/include
        vendor/imgui
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        std_module
        glfw
        spdlog::spdlog
        glm::glm
)