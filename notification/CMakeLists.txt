cmake_minimum_required(VERSION 3.10.0)
project(gsr-qt-notification VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core DBus Widgets Quick Gui QuickControls2)
find_package(LayerShellQt REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(${PROJECT_NAME} main.cpp)

qt_add_qml_module(${PROJECT_NAME}
    URI notification
    QML_FILES
        qml/Notification.qml
    SOURCES
        main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core Qt6::DBus Qt6::Widgets 
    Qt6::Quick Qt6::Gui Qt6::QuickControls2 
    LayerShellQtInterface 
    gsr-qt-settings
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})