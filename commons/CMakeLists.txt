cmake_minimum_required(VERSION 3.16)
project(FirehornCommons VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Network REQUIRED)

set(SOURCES_COMMONS
    src/ClientManager.cpp
    src/RequestBuilder.cpp
    src/Log.cpp
)

set(HEADERS_COMMONS
    include/ClientInterface.h
    include/ClientManager.h
    include/RequestBuilder.h 
    include/FieldUtil.h
    include/Log.h
    ERT_RF_Protocol_Interface/MacAdresses.h
    ERT_RF_Protocol_Interface/PacketDefinition.h
    ERT_RF_Protocol_Interface/ParameterDefinition.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES_COMMONS} ${HEADERS_COMMONS})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt6::Core
        Qt6::Network
)
