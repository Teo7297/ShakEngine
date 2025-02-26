cmake_minimum_required(VERSION 3.27)
project(ShakEngineTests LANGUAGES CXX)

# Create your test executable
add_executable(ShakEngineTests test_main.cpp)

# Link the ShakEngine library
target_link_libraries(ShakEngineTests PRIVATE ShakEngine)

# Add a test so that `ctest` will run your tests.
add_test(NAME ShakEngineTests COMMAND ShakEngineTests)