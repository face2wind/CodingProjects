cmake_minimum_required(VERSION 2.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project(datastructureandalgorithms)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Werror -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O2")
#set(CMAKE_CXX_FLAGS "-std=c++11 -Werror -Wall -ggdb")
include_directories(include)

add_executable(datastructureandalgorithms main.cpp)

install(TARGETS datastructureandalgorithms RUNTIME DESTINATION bin)
