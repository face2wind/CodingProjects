cmake_minimum_required(VERSION 3.3)
project(test_elegance)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -lpthread -Wall -Werror")
set(ELEGANCE_SRC_PATH "../Elegance/src/")
set(ELEGANCE_LIB_PATH "../Elegance/build")
#set(ELEGANCE_LIB "../../Elegance/cmake_build/build/libElegance.a")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.cc)

include_directories(
  src
  ${ELEGANCE_SRC_PATH}
  )
LINK_DIRECTORIES(${ELEGANCE_LIB_PATH})

add_executable(test_elegance ${SOURCE_FILES})

target_link_libraries(test_elegance libElegance.a) #${ELEGANCE_LIB})


