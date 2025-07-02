# In your root CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(FirehornProject)

# Global settings for consistency
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing for the entire project
enable_testing()

# Define component libraries first
add_subdirectory(commons)
add_subdirectory(GUI)
add_subdirectory(Server)

# Define all tests last, now that libraries are available
add_subdirectory(tests)
