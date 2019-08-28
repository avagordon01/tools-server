cmake_minimum_required(VERSION 3.10)

project(tools_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

add_library(server server.cc)
target_link_libraries(server -luv -lwebsockets)
add_executable(server-main server-main.cc)
target_link_libraries(server-main server)

add_library(harness harness.cc tcp.cc)
add_executable(harness-main harness-main.cc)
target_link_libraries(harness-main harness)
