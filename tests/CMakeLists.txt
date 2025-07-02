# In tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# --- Common Setup ---
find_package(Qt6 COMPONENTS Test Network Widgets REQUIRED)
find_package(SQLite3 REQUIRED)

# --- Google Test Setup (using FetchContent for portability) ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip # v1.10.0
)
FetchContent_MakeAvailable(googletest)

# --- Section 1: Server Unit Tests (gtest) ---
file(GLOB SERVER_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/server/*_tests.cpp")

foreach(_src IN LISTS SERVER_TEST_SOURCES)
    get_filename_component(_name ${_src} NAME_WE)
    
    add_executable(${_name} ${_src})
    
    target_link_libraries(${_name} PRIVATE
        gtest_main
        FirehornServer_lib
        SQLite::SQLite3
    )
    
    add_test(NAME server_${_name} COMMAND ${_name})
endforeach()


# --- Section 2: GUI/Server Integration Tests (QtTest) ---

# 2a. Create the base library for integration tests
add_library(base_integration_test_lib STATIC
    integration/base_integration_test.cpp
    integration/base_integration_test.h
)
target_compile_options(base_integration_test_lib PUBLIC -Wno-packed-bitfield-compat)
target_include_directories(base_integration_test_lib PUBLIC
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(base_integration_test_lib PUBLIC
    Qt6::Test
    FirehornGUI_lib
    FirehornServer_lib
    FirehornCommons
)

# 2b. Create the actual test executables
file(GLOB INTEGRATION_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/integration/*_test.cpp")

# --- THIS IS THE FIX ---
# Explicitly remove the base class source file from the list of executables to be built.
list(FILTER INTEGRATION_TEST_SOURCES EXCLUDE REGEX ".*base_integration_test\\.cpp$")
# ---------------------

foreach(_src IN LISTS INTEGRATION_TEST_SOURCES)
    get_filename_component(_name ${_src} NAME_WE)
    
    add_executable(${_name} ${_src})
    
    # Inherits all properties from the base library
    target_link_libraries(${_name} PRIVATE base_integration_test_lib)
    
    add_test(NAME integration_${_name} COMMAND ${_name})
endforeach()


add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests..."
)
