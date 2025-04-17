cmake_minimum_required(VERSION 3.30)

project(EasyEngineCore)

set(PROJECT_NAME EasyEngineCore)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        your_workbench_lib
        URL https://github.com/YiRanMushroom/Your_Workbench_Lib/archive/master.zip
)

FetchContent_MakeAvailable(your_workbench_lib)

add_library(${PROJECT_NAME}
        EZ/Core/Defines.ixx
)

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        EZ/Core/Defines.ixx
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        your_workbench_lib
)