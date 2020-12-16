cmake_minimum_required(VERSION 3.0)

project (OOP)

find_package(Boost COMPONENTS serialization REQUIRED)

add_executable (main main.cpp)

target_include_directories(main PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(main PUBLIC ${Boost_LIBRARIES})
