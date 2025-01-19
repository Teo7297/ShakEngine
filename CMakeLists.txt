cmake_minimum_required(VERSION 3.27)
project(ShakEngine LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Include all .cpp and .h files in src directory
file(GLOB_RECURSE SOURCES engine/src/*.cpp game/*.cpp)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE engine/include game)

target_link_libraries(main PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
    opengl32
)