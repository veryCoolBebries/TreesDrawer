cmake_minimum_required(VERSION 3.23)
project(TreesDrawer)

set(CMAKE_CXX_STANDARD 14)


file(GLOB SOURCES
        *.h
        *.cpp
        resources/*.*
        )

add_executable(TreesDrawer ${SOURCE_FILES} ${SOURCES} functions.h functions.cpp)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(TreesDrawer sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
