cmake_minimum_required(VERSION 2.8)
project(fileTransfer)
set (fileTransfer_VERSION_MAJOR 0)
set (fileTransfer_VERSION_MINOR 1)
set (fileTransfer_VERSION_PATCH 0)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake_config.h.in
  ${CMAKE_SOURCE_DIR}/src/cmake_config.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -lboost_system -lprotobuf -lpthread -Wall -Werror")

#file(GLOB SOURCE_FILES
#  src/libs/module/*.cpp
#  src/libs/network/*.cpp
#  src/libs/common/*.cpp
#  )
file(GLOB_RECURSE SOURCE_FILES ./src/libs/*.cpp ./src/file_transfer/*.cpp)
file(GLOB MAIN_CPP ./src/*.cpp)

include_directories(./src/libs ./src/file_transfer)

find_package(Boost COMPONENTS system filesystem thread log program_options REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
  include_directories(${PROTOBUF_INCLUDE_DIRS})
endif()

add_executable(fileTransfer ${SOURCE_FILES} ${MAIN_CPP})
target_link_libraries(fileTransfer ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES})

#target_link_libraries(myboost ${Boost_LIBRARIES} proto)

#add_subdirectory(protocol)

#PS1='\[\033[01;33m\]face2wind (\[\033[01;31m\]\W)\$\[\033[00m\] '
