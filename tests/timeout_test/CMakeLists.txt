cmake_minimum_required(VERSION 3.16.0)

project(librrp_timeout_test)

add_compile_options(-g)
add_compile_options(-O0)
add_compile_options(-Wall)
add_compile_options(-Wpedantic)


set(LOCAL ON)

add_executable(librrp_timeout_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_compile_features(librrp_timeout_test PRIVATE cxx_std_17)
target_include_directories(librrp_timeout_test PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(librrp_timeout_test PRIVATE librrp)
target_link_libraries(librrp_timeout_test PRIVATE libriccore)
target_link_libraries(librrp_timeout_test PRIVATE librnp)