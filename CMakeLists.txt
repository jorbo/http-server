cmake_minimum_required(VERSION 3.14)
project(http-server)
set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")


include_directories("include/")

add_executable(http-server ${SOURCE_FILES})

