cmake_minimum_required(VERSION 3.10.0)

project(gsr-qt-overlay VERSION 0.1.0 LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Core DBus Widgets Quick Gui QuickControls2)
find_package(LayerShellQt REQUIRED)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)
pkg_check_modules(XFIXES REQUIRED xfixes)

include_directories(${X11_INCLUDE_DIR} backends)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::DBus
        Qt::Widgets
        Qt::Quick
        Qt::Gui
        Qt::QuickControls2
        LayerShellQtInterface
        ${XFIXES_LIBRARIES}
        ${X11_LIBRARIES}
)

qt_add_qml_module(${PROJECT_NAME}
    URI GsrQt
    VERSION 1.0
    QML_FILES 
        qml/MainOverlay.qml
        qml/components/Header.qml
        qml/components/BigButton.qml
        qml/components/SmallIconButton.qml
        qml/components/MainButtonRow.qml
        qml/components/SmallSideButtonsColumn.qml
        qml/components/SubmenuListView.qml
    RESOURCES 
        images/gpu_screen_recorder_logo.png
        images/record.png
        images/replay.png
        images/stream.png
        images/settings.png
    SOURCES main.cpp
        backends/shutdownnotifier.h
        backends/shutdownnotifier.cpp
        backends/activewindow.h
        backends/activewindow.cpp

        dbusinterface.h
        dbusinterface.cpp
        
        x11hacks.h
        x11hacks.cpp
)


include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
