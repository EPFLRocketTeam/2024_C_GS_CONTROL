cmake_minimum_required(VERSION 3.16)

# Enable AUTOMOC globally for this directory
set(CMAKE_AUTOMOC ON)

# Find required Qt components for testing
find_package(Qt6 COMPONENTS Test Network Widgets REQUIRED)

# Enable testing
enable_testing()

# First, create a library for the base test class
add_library(base_integration_test_lib STATIC  # Renamed to avoid conflict
    integration/base_integration_test.cpp
    integration/base_integration_test.h
)

target_include_directories(base_integration_test_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/integration
    ${CMAKE_SOURCE_DIR}/GUI/include
    ${CMAKE_SOURCE_DIR}/GUI/src
    ${CMAKE_SOURCE_DIR}/Server/include
    ${CMAKE_SOURCE_DIR}/Server/src
    ${CMAKE_SOURCE_DIR}/commons/include
)

target_link_libraries(base_integration_test_lib PUBLIC
    Qt6::Test
    Qt6::Network
    Qt6::Widgets
    FirehornGUI_lib
    FirehornServer_lib
    FirehornCommons
)

# Suppress the GCC packed bitfield warning for base test class
target_compile_options(base_integration_test_lib PRIVATE
    -Wno-packed-bitfield-compat
)

# Add integration tests - exclude the base test file
file(GLOB INTEGRATION_TEST_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/integration/*_test.cpp"
)

# Remove base_integration_test.cpp from the list if it exists
list(FILTER INTEGRATION_TEST_SOURCES EXCLUDE REGEX ".*base_integration_test\\.cpp$")

foreach(_src IN LISTS INTEGRATION_TEST_SOURCES)
    get_filename_component(_name ${_src} NAME_WE)
    
    add_executable(${_name} ${_src})
    
    # Include the base test header
    target_include_directories(${_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/integration
        ${CMAKE_SOURCE_DIR}/GUI/include
        ${CMAKE_SOURCE_DIR}/GUI/src
        ${CMAKE_SOURCE_DIR}/Server/include
        ${CMAKE_SOURCE_DIR}/Server/src
        ${CMAKE_SOURCE_DIR}/commons/include
    )
    
    target_link_libraries(${_name} PRIVATE
        Qt6::Test
        Qt6::Network
        Qt6::Widgets
        FirehornGUI_lib      # Your GUI library
        FirehornServer_lib   # Your Server library  
        FirehornCommons      # Your commons library
        base_integration_test_lib # The base test library (renamed)
    )
    
    # Suppress the GCC packed bitfield warning
    target_compile_options(${_name} PRIVATE
        -Wno-packed-bitfield-compat
    )
    
    # Add as a test
    add_test(NAME ${_name} COMMAND ${_name})
    
    # Set test properties if needed
    set_tests_properties(${_name} PROPERTIES
        TIMEOUT 30
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach()

# Optional: Add a custom target to run only integration tests
add_custom_target(integration_tests
    COMMAND ${CMAKE_CTEST_COMMAND} -R ".*_test" --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running integration tests"
)
