cmake_minimum_required(VERSION 3.16)
project(FirehornServer VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find the required Qt6 components
find_package(Qt6 COMPONENTS Widgets Core Network SerialPort REQUIRED)

# Add CapsuleLib
add_library(CapsuleLib Capsule/src/capsule.h)

# Add FirehornCommons
# add_subdirectory(../commons FirehornCommons)

set(SOURCES_SERVER
    src/server.cpp
    src/RequestAdapter.cpp
    src/network/RequestHandler.cpp
)

set(HEADERS_SERVER
    include/Server.h 
    include/RequestHandler.h
    include/RequestAdapter.h
)

# Create the library
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES_SERVER} ${HEADERS_SERVER})

# Set include directories for the library
target_include_directories(${PROJECT_NAME}_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against the Qt6 modules and CapsuleLib for the library
target_link_libraries(${PROJECT_NAME}_lib 
    PUBLIC 
        Qt6::Widgets 
        Qt6::Core
        Qt6::Network
        Qt6::SerialPort
        CapsuleLib
        FirehornCommons
)

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the executable with the library
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        ${PROJECT_NAME}_lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
