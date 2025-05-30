cmake_minimum_required(VERSION 3.27)
project(ShakEngine LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # Trick to export everything. Better to only export what is needed in future.

include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.91.1
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR "${CMAKE_BINARY_DIR}/_deps/imgui-src" CACHE PATH "imgui top-level directory") # this is needed by imgui-sfml stupid cmakelists
message(STATUS "IMGUI_DIR: ${IMGUI_DIR}")

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

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
file(GLOB SOUND_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds/*.wav"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds/*.ogg"
)
file(GLOB MUSIC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/music/*.ogg"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/music/*.wav"
)
file(GLOB FONT_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts/*.ttf"
)
file(GLOB IMAGE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/*.jpg"
)
file(GLOB TEXTURE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*.jpg"
)
cmrc_add_resource_library(shak-resources
        ALIAS shak::rc
        NAMESPACE shak
        ${SHADER_FILES}
        ${SOUND_FILES}
        ${MUSIC_FILES}
        ${FONT_FILES}
        ${IMAGE_FILES}
        ${TEXTURE_FILES}
        )


# Include all .cpp and .h files in src directory
file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC "${SFML_SOURCE_DIR}/extlibs/headers/glad/include") # INFO: Check out other libraries in this directory if needed, like stbimage

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

enable_testing()
add_subdirectory(tests)