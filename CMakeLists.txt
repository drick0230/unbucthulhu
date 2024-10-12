cmake_minimum_required(VERSION 3.15)
project(unbucthulhu)

find_package(SFML 2.5.1 REQUIRED COMPONENTS Graphics Window System)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
