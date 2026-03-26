cmake_minimum_required(VERSION 3.15)
project(Networking LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/Server.cpp
    src/Socket.cpp
    src/HTML_METHODS.cpp
)

add_executable(Server ${SOURCES})



# Include directories
target_include_directories(Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Server)


# Compiler warnings
if(MSVC)
    target_compile_options(Server PRIVATE /W4 /permissive-)
    
else()
    target_compile_options(Server PRIVATE -Wall -Wextra -Wpedantic)
    
endif()