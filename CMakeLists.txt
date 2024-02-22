cmake_minimum_required(VERSION 3.22)
project(veryslot2_utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Core REQUIRED)

add_executable(veryslot2_utils main.cpp
        src/circular_buffer.cpp
        src/circular_buffer.h
        src/cb_iterator.cpp
        src/cb_iterator.h)
target_include_directories(veryslot2_utils PRIVATE src/)
target_link_libraries(veryslot2_utils Qt6::Core)