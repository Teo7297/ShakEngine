cmake_minimum_required(VERSION 3.27)
project(ShakEngineTests LANGUAGES CXX)

# Create separate test executables
add_executable(TestMain test_main.cpp)
add_executable(TestSplash test_splash.cpp)

# Link the ShakEngine library to each test
target_link_libraries(TestMain PRIVATE ShakEngine)
target_link_libraries(TestSplash PRIVATE ShakEngine)

# Add tests so that `ctest` will run them
add_test(NAME TestMain COMMAND TestMain)
add_test(NAME TestSplash COMMAND TestSplash)
