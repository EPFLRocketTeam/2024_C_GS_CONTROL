cmake_minimum_required(VERSION 3.16)
project(FirehornLauncher VERSION 1.0)

add_subdirectory(GUI)
add_subdirectory(Server)

# Find Qt packages
find_package(Qt6 COMPONENTS Widgets Core Network SerialPort REQUIRED)
set(CMAKE_AUTOMOC ON) #
# Add the main executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the main executable with projects and Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets 
    Qt6::Core
    Qt6::Network
    Qt6::SerialPort
)

# Add pthread for threading support
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
# If you want to run it in a separate thread
target_link_libraries(${PROJECT_NAME} PRIVATE FirehornGUI_lib)
# If you want to run it in a separate thread
target_link_libraries(${PROJECT_NAME} PRIVATE FirehornServer_lib)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        FirehornCommons
)
