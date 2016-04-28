project(baifenserver)

file(GLOB LIB_SOURCES
 net/*.cpp
 modules/*.cpp
 database/*.cpp
 cache/*.cpp
)

#target_link_libraries(baifenserver -lmysqlclient)

include_directories(./net/ ./modules ./database ./cache)

set(CMAKE_CXX_FLAGS "-std=c++11 -Werror -Wall -ggdb")

add_executable(baifenserver
${LIB_SOURCES}
main.cpp
)

target_link_libraries(baifenserver mysqlclient pthread)

install(TARGETS baifenserver RUNTIME DESTINATION bin)

#add_subdirectory(net/protocol)
