cmake_minimum_required(VERSION 3.10.0)
project(gsr-qt VERSION 0.1.0 LANGUAGES C CXX)

add_compile_definitions(APP_ID="com.github.relative.gsrqt")
add_compile_definitions(OVERLAY_APP_ID="com.github.relative.gsrqt.overlay")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

add_subdirectory(overlay)
add_subdirectory(daemon)
add_subdirectory(settings)