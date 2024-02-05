cmake_minimum_required(VERSION 3.15)

project(JsonParse)

set(SRC_LIST JsonParse.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/rapidjson)