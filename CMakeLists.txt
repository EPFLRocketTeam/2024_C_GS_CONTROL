cmake_minimum_required(VERSION 3.16)
project(FirehornLauncher VERSION 1.0)

add_subdirectory(GUI)
add_subdirectory(Server)


add_custom_target(run_gui
    COMMENT "Run the GUI and the server together"
    COMMAND ${CMAKE_COMMAND} -E echo "Running GUI and Server..."
    COMMAND ${CMAKE_COMMAND} -E env ./GUI/FirehornGUI & ./Server/FirehornServer
    DEPENDS FirehornGUI FirehornServer
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)


