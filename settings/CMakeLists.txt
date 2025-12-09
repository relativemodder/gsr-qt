cmake_minimum_required(VERSION 3.10.0)
project(gsr-qt-settings VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

add_library(${PROJECT_NAME} STATIC
    gsrsettings.cpp
    gsrsettings.h
)

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})