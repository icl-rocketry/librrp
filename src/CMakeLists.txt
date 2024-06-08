cmake_minimum_required(VERSION 3.16.3)

add_library(librrp STATIC)

#collect source files
file(GLOB_RECURSE LIBRRP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(librrp PUBLIC ${LIBRRP_SRC})

#get include dcirectories relative to the current directory, i.e we have the prefix librnp/...
target_include_directories(librrp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


