cmake_minimum_required(VERSION 3.16)
project(Project-3)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp Variable.cpp Function.cpp GlobalInfo.cpp Logger.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
