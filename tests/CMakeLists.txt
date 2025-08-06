# In tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# --- Common Setup ---
find_package(Qt6 COMPONENTS Test Network Widgets REQUIRED)
find_package(SQLite3 REQUIRED)

# --- Google Test Setup (using FetchContent for portability) ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(test_helper_lib STATIC
    helpers/LogSpySignals.h)


# --- Section 1: Server Unit Tests (gtest) ---
file(GLOB SERVER_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/server/*_tests.cpp")

foreach(_src IN LISTS SERVER_TEST_SOURCES)
    get_filename_component(_name ${_src} NAME_WE)
    
    add_executable(${_name} ${_src})
    
    target_include_directories(${_name}
    PRIVATE
      ${CMAKE_SOURCE_DIR}/Server/include
  )
    target_include_directories(${_name}
  PRIVATE
    ${googletest_SOURCE_DIR}/googletest/include
)
    target_link_libraries(${_name} PRIVATE
        GTest::gtest_main
        FirehornServer_lib
        SQLite::SQLite3
        data_storage_lib
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

target_compile_definitions(base_integration_test_lib PUBLIC
    -DPROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(base_integration_test_lib PUBLIC
    Qt6::Test
    FirehornGUI_lib
    FirehornServer_lib
    FirehornCommons
    test_helper_lib
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
    set_tests_properties(integration_${_name}
  PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=minimal"
    )
endforeach()


add_custom_target(check
  COMMENT "Building everything then running all tests…"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  # 1) build any out-of-date targets
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
  # 2) run ctest with verbose output on failure
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)
