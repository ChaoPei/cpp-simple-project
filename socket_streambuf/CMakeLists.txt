cmake_minimum_required(VERSION 3.10)

project(tcp_stream_buffer)

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

add_executable(
    client
    client.cpp
    tcp_stream.cpp
)

add_executable(
    server
    server.cpp
    tcp_stream.cpp
)