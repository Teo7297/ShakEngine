cmake_minimum_required(VERSION 3.27)
project(ShakEngine LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.91.1
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src" CACHE PATH "imgui top-level directory") # this is needed by imgui-sfml stupid cmakelists

FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

# This library embeds all the resources in the executable as cpp compiled files.
# Very useful for shaders.
include(cmake/CMakeRC.cmake)

file(GLOB SHADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.vs"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.fs"
)
cmrc_add_resource_library(shak-resources
        ALIAS shak::rc
        NAMESPACE shak
        ${SHADER_FILES})


# Include all .cpp and .h files in src directory
file(GLOB_RECURSE SOURCES engine/src/*.cpp game/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC engine/include game)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
    opengl32
    ImGui-SFML::ImGui-SFML
    shak-resources shak::rc
)