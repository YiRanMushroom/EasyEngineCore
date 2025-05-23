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

# Generated
file(GLOB_RECURSE Generated_MODULES
        Easy/Generated/*.ixx
        Easy/Generated/*.cppm
)
file(GLOB_RECURSE Generated_SOURCES
        Easy/Generated/*.cpp
)

# ImGui
file(GLOB_RECURSE ImGui_MODULES
        Easy/ImGui/*.ixx
        Easy/ImGui/*.cppm
)
file(GLOB_RECURSE ImGui_SOURCES
        Easy/ImGui/*.cpp
)

# Math
file(GLOB_RECURSE Math_MODULES
        Easy/Math/*.ixx
        Easy/Math/*.cppm
)
file(GLOB_RECURSE Math_SOURCES
        Easy/Math/*.cpp
)

# Physics
file(GLOB_RECURSE Physics_MODULES
        Easy/Physics/*.ixx
        Easy/Physics/*.cppm
)
file(GLOB_RECURSE Physics_SOURCES
        Easy/Physics/*.cpp
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

# Scene
file(GLOB_RECURSE Scene_MODULES
        Easy/Scene/*.ixx
        Easy/Scene/*.cppm
)
file(GLOB_RECURSE Scene_SOURCES
        Easy/Scene/*.cpp
)

# Scripting
file(GLOB_RECURSE Scripting_MODULES
        Easy/Scripting/*.ixx
        Easy/Scripting/*.cppm
)
file(GLOB_RECURSE Scripting_SOURCES
        Easy/Scripting/*.cpp
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

file(GLOB_RECURSE Windows_MODULES
        Easy/Platform/Windows/*.ixx
        Easy/Platform/Windows/*.cppm
)

file(GLOB_RECURSE Windows_SOURCES
        Easy/Platform/Windows/*.cpp
)

# Project
file(GLOB_RECURSE Project_MODULES
        Easy/Project/*.ixx
        Easy/Project/*.cppm
)
file(GLOB_RECURSE Project_SOURCES
        Easy/Project/*.cpp
)

file(GLOB ImGuiGLFW_Sources
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/ImGuizmo/GraphEditor.cpp
        vendor/ImGuizmo/ImCurveEdit.cpp
        vendor/ImGuizmo/ImGradient.cpp
        vendor/ImGuizmo/ImGuizmo.cpp
        vendor/ImGuizmo/ImSequencer.cpp
)

add_library(${PROJECT_NAME}
        ${VendorFiles}
        ${Core_MODULES}
        ${Core_SOURCES}
        ${Event_MODULES}
        ${Event_SOURCES}
        ${Generated_MODULES}
        ${Generated_SOURCES}
        ${ImGui_MODULES}
        ${ImGui_SOURCES}
        ${Math_MODULES}
        ${Math_SOURCES}
        ${Physics_MODULES}
        ${Physics_SOURCES}
        ${OPENGL_SOURCES}
        ${OPENGL_MODULES}
        ${Platform_MODULES}
        ${Platform_SOURCES}
        ${Project_MODULES}
        ${Project_SOURCES}
        ${Renderer_MODULES}
        ${Renderer_SOURCES}
        ${Scene_MODULES}
        ${Scene_SOURCES}
        ${Scripting_MODULES}
        ${Scripting_SOURCES}
        ${Vendor_MODULES}
        ${Vendor_SOURCES}
        ${Windows_MODULES}
        ${Windows_SOURCES}
        ${ImGuiGLFW_Sources}
        Easy/mod.ixx
)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        "EZ_DEBUG=1"
        "EZ_PLATFORM_WINDOWS"
        "EZ_RENDERER_OPENGL"
        "_CRT_SECURE_NO_WARNINGS"
)

target_compile_options(${PROJECT_NAME}
        PUBLIC
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
        ${Generated_MODULES}
        ${ImGui_MODULES}
        ${Math_MODULES}
        ${Physics_MODULES}
        ${OPENGL_MODULES}
        ${Platform_MODULES}
        ${Project_MODULES}
        ${Renderer_MODULES}
        ${Scene_MODULES}
        ${Scripting_MODULES}
        ${Vendor_MODULES}
        ${Windows_MODULES}
        Easy/mod.ixx
)

add_subdirectory(vendor/std_module)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/SPIRV-Headers)
add_subdirectory(vendor/SPIRV-Tools)
add_subdirectory(vendor/SPIRV-Cross)
add_subdirectory(vendor/glslang)
add_subdirectory(vendor/yaml-cpp)

add_subdirectory(vendor/LuaJIT)

set(SOL2_LUA_VERSION "luajit-2.1")
add_subdirectory(vendor/sol2)

add_subdirectory(vendor/jni-bind)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Build the Box2D unit tests" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Build the Box2D testbed" FORCE)
add_subdirectory(vendor/box2d)
set(SHADERC_SKIP_TESTS ON CACHE BOOL "Skip building Shaderc tests")
set(SHADERC_ENABLE_TESTS OFF CACHE BOOL "Enable Shaderc tests")
add_subdirectory(vendor/shaderc)

add_subdirectory(vendor/msdf-atlas-gen)
add_subdirectory(vendor/libcuckoo)

#target_precompile_headers(${PROJECT_NAME}
#        PUBLIC
#        ${PreCompiledHeaders}
#)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        Easy/PCH
        Easy
        vendor/glad/include
        vendor/imgui
        vendor/SPIRV-Cross
        vendor/ImGuizmo
        vendor/stb_image
        vendor/msdf-atlas-gen/msdf-atlas-gen
        vendor/msdf-atlas-gen/msdfgen
        vendor/msdf-atlas-gen/msdfgen/include
#        vendor/mono/include
)


target_link_libraries(${PROJECT_NAME}
        PUBLIC
        std_module
        glfw
        spdlog::spdlog
        glm::glm
        shaderc
        SPIRV
        SPIRV-Headers::SPIRV-Headers
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-cpp
        spirv-cross-reflect
        spirv-cross-msl
        spirv-cross-hlsl
        spirv-cross-util
        msdf-atlas-gen
        yaml-cpp::yaml-cpp
        box2d
        libluajit
        sol2
        jni_bind
        libcuckoo::libcuckoo
)