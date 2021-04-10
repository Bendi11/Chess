cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
project(Chess)

if(NOT DEFINED CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Release")
endif()

set(SOURCES
    "src/bitboard.cpp"
    "src/main.cpp"
)

include_directories(
    "src/include"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
