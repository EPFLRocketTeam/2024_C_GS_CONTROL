# In your root CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(FirehornProject)

# Global settings for consistency
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("-Wno-packed-bitfield-compat")

# Enable testing for the entire project
enable_testing()

add_library(config_interface INTERFACE)

# 2) Define the macro as a PUBLIC compile definition
#    This makes -DPROJECT_ROOT_PATH="…" available to anything that links to it.
target_compile_definitions(config_interface
  INTERFACE
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
)

# Define component libraries first
add_subdirectory(commons)
add_subdirectory(GUI)
add_subdirectory(Server)

# Define all tests last, now that libraries are available
add_subdirectory(tests)
