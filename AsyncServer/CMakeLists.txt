cmake_minimum_required(VERSION 3.15)

project(AsyncServer)

find_package(Boost)

include_directories(${Boost_INCLUDE_DIRS})

set(SRC_LIST SingleT.hpp AsyncServer.cpp
 IoContextPool.h IoContextPool.cpp
 IoContextProxy.h IoContextProxy.cpp
 LogicSystem.h LogicSystem.cpp 
 Session.h Session.cpp 
 TcpServer.h TcpServer.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST}) 
