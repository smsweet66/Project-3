cmake_minimum_required(VERSION 3.16)
project(Project-3)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp Variable.cpp GlobalInfo.cpp Function.cpp ClassDef.cpp)
target_precompile_headers(${PROJECT_NAME} PUBLIC pch.hpp)
