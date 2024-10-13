cmake_minimum_required(VERSION 3.15)

project(${CMAKE_PROJECT_NAME} CXX)

find_package(SFML CONFIG REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfml::sfml)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
