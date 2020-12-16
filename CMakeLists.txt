cmake_minimum_required(VERSION 3.11)

project (OOP)

find_package(Boost COMPONENTS serialization REQUIRED)

add_executable (main main.cpp)