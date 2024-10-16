cmake_minimum_required(VERSION 3.15)

project(${CMAKE_PROJECT_NAME} CXX)

message(STATUS "CONAN_BIN_DIRS_PROTOBUF: ${CMAKE_BINARY_DIR}")

find_package(protobuf CONFIG REQUIRED)

FILE(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(${CMAKE_PROJECT_NAME} ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE protobuf::protobuf)


set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${PROTO_HDRS})
install(TARGETS ${CMAKE_PROJECT_NAME})
