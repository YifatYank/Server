cmake_minimum_required(VERSION 3.6)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BOOST_ROOT CACHE PATH "/usr/include/boost/")
find_package(Boost 1.58.0 COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp BfsGrid.cpp BfsGrid.h Cab.cpp Cab.h Definitions.h Driver.cpp Driver.h Enums.h Grid.cpp Grid.h GridNode.cpp GridNode.h Luxury.cpp Luxury.h Passenger.cpp Passenger.h Point.h Point.cpp PointNode.cpp PointNode.h Standart.cpp Standart.h TaxiCenter.cpp TaxiCenter.h Trip.cpp Trip.h Socket.cpp Socket.h Udp.cpp Udp.h)
add_executable(Server ${SOURCE_FILES})
target_link_libraries(Server ${Boost_LIBRARIES})