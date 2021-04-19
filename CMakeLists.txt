cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
project(Chess)

include(deps.cmake) #Download all dependencies of the project

if(NOT DEFINED CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Release")
endif()

set(SOURCES
    "src/board.cpp"
    "src/main.cpp"
)

include_directories(
    "src/include"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
