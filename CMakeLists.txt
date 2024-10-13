cmake_minimum_required(VERSION 3.15)
project(unbucthulhu CXX)

find_package(SFML CONFIG REQUIRED)


add_executable(unbucthulhu src/main.cpp)

target_link_libraries(unbucthulhu PRIVATE sfml::sfml)


install(TARGETS unbucthulhu DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
