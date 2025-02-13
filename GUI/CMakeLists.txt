cmake_minimum_required(VERSION 3.16)
project(FirehornGUI VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Add subdirectory for tests if needed
add_subdirectory(tests)

# Find the required Qt6 components
find_package(Qt6 COMPONENTS Widgets Core Network Svg Concurrent REQUIRED)

# Add your source files
set(SOURCES_GUI
    src/components/DataLabel.cpp
    src/components/DraggableButton.cpp
    src/components/ToggleButton.cpp
    src/components/ValveButton.cpp
    src/components/ValveControlButton.cpp
    src/json/jsoncpp.cpp
    src/views/ControlPannelView.cpp 
    src/views/DataView.cpp
    src/views/GSManagerView.cpp
    src/views/SerialView.cpp
    src/views/TelemetryView.cpp
    src/views/TimerView.cpp
    src/views/ValveControlView.cpp
    src/MainWindow.cpp
)

set(HEADERS_GUI
    src/Setup.h
    src/FileLocation.h
    include/components/DataLabel.h
    include/components/DraggableButton.h
    include/components/ToggleButton.h
    include/components/ValveButton.h
    include/components/ValveControlButton.h
    include/json/json-forwards.h
    include/json/json.h
    include/ControlPannelView.h
    include/DataView.h
    include/GSManagerView.h
    include/MainWindow.h
    include/SerialView.h
    include/TelemetryView.h
    include/TimerView.h    
    include/ValveControlView.h
)

# Create the library
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES_GUI} ${HEADERS_GUI})

# add_subdirectory(../commons FirehornCommons)

# Set include directories for the library
target_include_directories(${PROJECT_NAME}_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link against the Qt6 modules for the library
target_link_libraries(${PROJECT_NAME}_lib 
    PUBLIC 
        Qt6::Widgets 
        Qt6::Core
        Qt6::Network
        Qt6::Svg
        Qt6::Concurrent
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
