cmake_minimum_required(VERSION 3.30)

project(EasyEngineCore)

set(PROJECT_NAME EasyEngineCore)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # use static runtime library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else ()
endif ()

file(GLOB_RECURSE Core_MODULES
        Easy/Core/*.ixx
)

file(GLOB_RECURSE Core_SOURCES
        Easy/Core/*.cpp
)

file(GLOB_RECURSE Event_MODULES
        Easy/Events/*.ixx
)

file(GLOB_RECURSE Event_SOURCES
        Easy/Events/*.cpp
)

# Generated
file(GLOB_RECURSE Generated_MODULES
        Easy/Generated/*.ixx
)
file(GLOB_RECURSE Generated_SOURCES
        Easy/Generated/*.cpp
)

# ImGui
file(GLOB_RECURSE ImGui_MODULES
        Easy/ImGui/*.ixx
)
file(GLOB_RECURSE ImGui_SOURCES
        Easy/ImGui/*.cpp
)

# Math
file(GLOB_RECURSE Math_MODULES
        Easy/Math/*.ixx
)
file(GLOB_RECURSE Math_SOURCES
        Easy/Math/*.cpp
)

# Physics
file(GLOB_RECURSE Physics_MODULES
        Easy/Physics/*.ixx
)
file(GLOB_RECURSE Physics_SOURCES
        Easy/Physics/*.cpp
)

# Platform
file(GLOB_RECURSE Platform_MODULES
        Easy/Platform/*.ixx
)
file(GLOB_RECURSE Platform_SOURCES
        Easy/Platform/*.cpp
)

# Renderer
file(GLOB_RECURSE Renderer_MODULES
        Easy/Renderer/*.ixx
)
file(GLOB_RECURSE Renderer_SOURCES
        Easy/Renderer/*.cpp
)

# Scene
file(GLOB_RECURSE Scene_MODULES
        Easy/Scene/*.ixx
)
file(GLOB_RECURSE Scene_SOURCES
        Easy/Scene/*.cpp
)

# Scripting
file(GLOB_RECURSE Scripting_MODULES
        Easy/Scripting/*.ixx
)
file(GLOB_RECURSE Scripting_SOURCES
        Easy/Scripting/*.cpp
)

# Vendor
file(GLOB_RECURSE Vendor_MODULES
        Easy/Vendor/*.ixx
)
file(GLOB_RECURSE Vendor_SOURCES
        Easy/Vendor/*.cpp
)

file(GLOB VendorFiles vendor/glad/src/glad.c)

file(GLOB_RECURSE OPENGL_MODULES
        Easy/Platform/Impl/OpenGL/*.ixx
)

file(GLOB_RECURSE OPENGL_SOURCES
        Easy/Platform/Impl/OpenGL/*.cpp
)

file(GLOB_RECURSE Windows_MODULES
        Easy/Platform/Impl/Windows/*.ixx
)

file(GLOB_RECURSE Windows_SOURCES
        Easy/Platform/Impl/Windows/*.cpp
)

# Project
file(GLOB_RECURSE Project_MODULES
        Easy/Project/*.ixx
)
file(GLOB_RECURSE Project_SOURCES
        Easy/Project/*.cpp
)

file(GLOB ImGuiGLFW_Sources
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
        ${Windows_SOURCES}
        ${Windows_MODULES}
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
        ${ImGuiGLFW_Sources}
        vendor/imgui/imgui_opengl3.ixx
        Easy/mod.ixx
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor /Wv:19.23)
endif ()

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        "EZ_DEBUG=1"
        "EZ_PLATFORM_WINDOWS"
        "EZ_RENDERER_OPENGL"
        "_CRT_SECURE_NO_WARNINGS"
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
        vendor/imgui/imgui_opengl3.ixx
        Easy/mod.ixx
)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/SPIRV-Headers)
add_subdirectory(vendor/SPIRV-Tools)
add_subdirectory(vendor/SPIRV-Cross)
set(ENABLE_PCH OFF)
add_subdirectory(vendor/glslang)
add_subdirectory(vendor/yaml-cpp)
add_subdirectory(vendor/imgui)

add_subdirectory(vendor/LuaJIT)

set(SOL2_LUA_VERSION "luajit-2.1")
add_subdirectory(vendor/sol2)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Build the Box2D unit tests" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Build the Box2D testbed" FORCE)
add_subdirectory(vendor/box2d)
set(SHADERC_SKIP_TESTS ON CACHE BOOL "Skip building Shaderc tests")
set(SHADERC_ENABLE_TESTS OFF CACHE BOOL "Enable Shaderc tests")
add_subdirectory(vendor/shaderc)

add_subdirectory(vendor/msdf-atlas-gen)

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
)


target_link_libraries(${PROJECT_NAME}
        PUBLIC
        glfw
        spdlog::spdlog
        imgui::imgui
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
)