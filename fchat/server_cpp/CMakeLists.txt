cmake_minimum_required(VERSION 2.8)
project(Elegance)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -lboost_system -lprotobuf -lpthread -Wall -Werror")
set(ELEGANCE_SRC_PATH "../../Elegance/src/")
set(ELEGANCE_LIB_PATH "../../../Elegance/cmake_build/build")
#set(ELEGANCE_LIB "../../Elegance/cmake_build/build/libElegance.a")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.cc)

find_package(Boost COMPONENTS system filesystem thread log program_options REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
  include_directories(${PROTOBUF_INCLUDE_DIRS})
endif()

include_directories(
  src
  ${ELEGANCE_SRC_PATH}
  )
LINK_DIRECTORIES(${ELEGANCE_LIB_PATH})

add_executable(server ${SOURCE_FILES})

target_link_libraries(server ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} libElegance.a) #${ELEGANCE_LIB})


