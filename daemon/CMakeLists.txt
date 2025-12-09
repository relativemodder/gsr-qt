cmake_minimum_required(VERSION 3.10.0)

project(gsr-qt-daemon-service VERSION 0.1.0 LANGUAGES C CXX)

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(KF6 REQUIRED COMPONENTS GlobalAccel)
find_package(Qt6 REQUIRED COMPONENTS Core DBus Widgets)
find_package(X11 REQUIRED)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    main.cpp

    shortcuts.h
    shortcuts.cpp

    dbusinterface.h
    dbusinterface.cpp

    notificationservice.h
    notificationservice.cpp

    processes/overlayprocess.h
    processes/overlayprocess.cpp

    processes/instanceprocess.h
    processes/instanceprocess.cpp

    processes/gsrcli.h
    processes/gsrcli.cpp
    processes/gsrargs.h
    processes/gsrargs.cpp

    actions/toggleshowaction.cpp
    actions/toggleshowaction.h

    actions/togglerecordaction.h
    actions/togglerecordaction.cpp

    windowing/x11fs.h
    windowing/x11fs.cpp

    windowing/activewindow.h
    windowing/activewindow.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::DBus
        Qt::Widgets
        KF6::GlobalAccel
        ${X11_LIBRARIES}
        gsr-qt-settings
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