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
    )


set(ERT_PROTOCOL_INTERFACE_FILES
    ERT_RF_Protocol_Interface/Protocol.h
    ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h
    ERT_RF_Protocol_Interface/PacketDefinition_Icarus.h
)

function(add_commons_library lib_name definition)
    add_library(${lib_name} STATIC ${ERT_PROTOCOL_INTERFACE_FILES})

    target_compile_definitions(${lib_name} PUBLIC ${definition})
    target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ERT_RF_Protocol_Interface)
endfunction()

add_library(CommonsLib STATIC ${SOURCES_COMMONS} ${HEADERS_COMMONS})

target_include_directories(CommonsLib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CommonsLib
    PUBLIC 
        Qt6::Core
        Qt6::Network
)

# Create FirehornCommons
add_commons_library(FirehornProtocol RF_PROTOCOL_FIREHORN)

# Create IcarusCommons
add_commons_library(IcarusProtocol RF_PROTOCOL_ICARUS)
