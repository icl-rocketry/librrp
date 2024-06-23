cmake_minimum_required(VERSION 3.16.0)

project(tdma_test)

add_compile_options(-g)
add_compile_options(-O0)
add_compile_options(-Wall)
add_compile_options(-Wpedantic)

set(LOCAL ON)

# Ensure libriccore and librnp are included
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/libriccore ${CMAKE_CURRENT_BINARY_DIR}/libriccore)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/librnp ${CMAKE_CURRENT_BINARY_DIR}/librnp)

# Add the main project source directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../..)

add_executable(tdma_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_compile_features(tdma_test PRIVATE cxx_std_17)
target_include_directories(tdma_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tdma_test PRIVATE libriccore)
target_link_libraries(tdma_test PRIVATE librnp)



