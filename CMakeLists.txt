# Set minimum CMake version
cmake_minimum_required(VERSION 3.16.3)

# Define the LOCAL option
option(LOCAL "Compile for UNIX" OFF)

# Check if we are on ESP32 platform
if (ESP_PLATFORM)
    set(LOCAL OFF)
endif()

if (ESP_PLATFORM)
    idf_component_register(
        INCLUDE_DIRS src lib/libriccore/src lib/librnp/src
        REQUIRES librnp
    )
    # Add subdirectories
    add_subdirectory(src)
    add_subdirectory(lib/libriccore)
    add_subdirectory(lib/librnp)

    # Link the original cmake librnp library to the component library
    target_link_libraries(${COMPONENT_LIB} INTERFACE librrp)
    return()
endif()

# Declare project
project(librrp LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/libriccore/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/librnp/src)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(lib/libriccore)
add_subdirectory(lib/librnp)

# Link libraries
target_link_libraries(librrp PRIVATE platform)

# Set the preprocessor definition based on the LOCAL option
if(LOCAL)
    target_compile_definitions(librrp PRIVATE LIBRICCORE_UNIX)
endif()

# Add tests
option(TESTS "Enable building of tests" OFF)
if (TESTS)
    #add_subdirectory(tests/tdma_test)
    add_executable(tdma_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
    target_compile_features(tdma_test PRIVATE cxx_std_17)
    target_include_directories(tdma_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(tdma_test PRIVATE libriccore)
    target_link_libraries(tdma_test PRIVATE librnp)
endif()
