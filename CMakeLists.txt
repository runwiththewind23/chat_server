# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(chat)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Include directory
include_directories(include)

# Define source files
set(CLIENT_SRC "src/client.cpp" "src/handleclient.cpp")
set(SERVER_SRC "src/server.cpp" "src/handleserver.cpp" "src/lock.cpp" "src/mysql_connection_pool.cpp" "src/redis_connection_pool.cpp" "src/ThreadPool.cpp") 
# Add the client executable
add_executable(client ${CLIENT_SRC})
target_link_libraries(client pthread) # Link libraries if client needs them

# Add the server executable
add_executable(server ${SERVER_SRC})

# Link libraries for the server
target_link_libraries(server mysqlclient pthread hiredis)
