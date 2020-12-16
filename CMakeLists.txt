cmake_minimum_required(VERSION 3.0)

project (OOP)

add_definitions(-std=c++11)

find_package(Boost COMPONENTS serialization REQUIRED)

add_executable (a.out main.cpp)

target_include_directories(a.out PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(a.out PUBLIC ${Boost_LIBRARIES})
