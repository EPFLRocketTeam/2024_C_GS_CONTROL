cmake_minimum_required(VERSION 3.16)
project(FirehornServer VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find the required Qt6 components
find_package(Qt6 COMPONENTS Widgets Core Network SerialPort REQUIRED)

find_package(Qt6HttpServer)
# Add CapsuleLib
add_library(CapsuleLib Capsule/src/capsule.h)

set(SOURCES_SERVER
    src/server.cpp
    src/RequestAdapter.cpp
    src/network/RequestHandler.cpp
    src/data_storage.cpp
)

set(HEADERS_SERVER
    src/ServerSetup.h
    include/Server.h 
    include/RequestHandler.h
    include/RequestAdapter.h
    include/sqlite_orm.h
    include/data_storage.h
)

function(add_server_exec exec_name protocol_name definition)
    
    # Create the library
    add_library(${exec_name}_lib STATIC ${SOURCES_SERVER} ${HEADERS_SERVER})
    target_compile_definitions(${exec_name}_lib PRIVATE ${definition}=1)

    # Set include directories for the library
    target_include_directories(${exec_name}_lib 
        PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )


    # Link against the Qt6 modules and CapsuleLib for the library
    target_link_libraries(${exec_name}_lib 
        PUBLIC 
            Qt6::Widgets 
            Qt6::Core
            Qt6::Network
            Qt6::SerialPort
            Qt6HttpServer
            sqlite3
            CapsuleLib
            CommonsLib
            ${protocol_name}
            config_interface

    )

    # Create the executable
    add_executable(${exec_name} src/main.cpp)
    # Link the executable with the library
    target_link_libraries(${exec_name} 
        PRIVATE 
            ${exec_name}_lib
    )

    set_target_properties(${exec_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )

endfunction()

add_server_exec(FirehornServer FirehornProtocol RF_PROTOCOL_FIREHORN)
add_server_exec(IcarusServer IcarusProtocol RF_PROTOCOL_ICARUS)
##### data_storage stuff #####

find_package(SQLite3 REQUIRED)

SET(HEADERS_STORAGE
    include/data_storage.h
    include/sqlite_orm.h
)
SET(SOURCES_STORAGE
    src/data_storage.cpp
)

add_library(data_storage_lib STATIC ${SOURCES_STORAGE} ${HEADERS_STORAGE})

# Set include directories for the library
target_include_directories(data_storage_lib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(data_storage_lib PRIVATE RF_PROTOCOL_FIREHORN=1)

target_link_libraries(data_storage_lib
    PRIVATE
        config_interface
                Qt6::Core


)
