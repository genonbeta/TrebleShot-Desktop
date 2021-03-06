cmake_minimum_required(VERSION 3.2.0)  # 3.1.0

project(TrebleShot)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Minimum C++11
set(CMAKE_CXX_STANDARD 11)

# Include Qt basic functions
if (${QTLIBDIR})
    MESSAGE(DEBUG "QTLIBDIR: ${QTLIBDIR}")
    set(QT_QMAKE_EXECUTABLE "${QTLIBDIR}/bin/qmake")
    MESSAGE(DEBUG "QT_QMAKE_EXECUTABLE: ${QT_QMAKE_EXECUTABLE}")
    set(QT_ROOT_DIR "${QTLIBDIR}")
    set(QT_DIR "${QTLIBDIR}")
    set(Qt5_DIR "${QTLIBDIR}")
    set(Qt5Core_DIR "${QTLIBDIR}/lib/cmake/Qt5Core")  # /lib/cmake/Qt5Core  /include/Qt5Core
endif ()
include(QtCommon)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Make this a GUI application on Windows
if (WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif ()

set(PROJECT_NAME "TrebleShot")
set(PROJECT_EXECUTABLE "trebleshot")
set(PROJECT_DESCRIPTION "Multi-platform file-sharing software")
set(PROJECT_AUTHOR "Genonbeta")
set(PROJECT_URL "https://github.com/genonbeta/TrebleShot-Desktop")

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(PROJECT_VERSION_CODE 91)

set(LINKED_LIBRARY_NAME "linked_libraries")
set(LINKED_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${LINKED_LIBRARY_NAME})

# Find the Qt5 libraries
find_package(Qt5 COMPONENTS Core REQUIRED HINTS ${QTLIBDIR})  #  ${QTLIBDIR}/lib/cmake
find_package(Qt5 COMPONENTS Gui REQUIRED HINTS ${QTLIBDIR})
find_package(Qt5 COMPONENTS Network REQUIRED HINTS ${QTLIBDIR})
find_package(Qt5 COMPONENTS Sql REQUIRED HINTS ${QTLIBDIR})
find_package(Qt5 COMPONENTS Widgets REQUIRED HINTS ${QTLIBDIR})
find_package(KF5DNSSD NO_MODULE)

IF (Qt5Core_FOUND)
    MESSAGE(STATUS "Found Qt ${Qt5Core_VERSION_STRING}")

    # FIX: Qt was built with -reduce-relocations
    if (Qt5_POSITION_INDEPENDENT_CODE)
        SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif ()

    # guess plugins and libraries directory
    set(QT_DIR "${Qt5Core_DIR}/../../..")
    MESSAGE(DEBUG "QT_DIR: ${QT_DIR}")
    set(QT_BINARY_DIR "${QT_DIR}/bin")
    set(QT_PLUGINS_DIR "${QT_DIR}/plugins")
    get_target_property(QT_LIBRARY_DIR Qt5::Core LOCATION)
    get_filename_component(QT_LIBRARY_DIR ${QT_LIBRARY_DIR} PATH)
ELSE ()
    MESSAGE(FATAL_ERROR "No Qt5 found!")
ENDIF ()

configure_file(src/build.h.in src/build.h)

add_project_meta(META_FILES_TO_INCLUDE)

qt5_add_resources(QRCS
        assets/resources.qrc)

qt5_wrap_ui(UI_HEADERS
        src/ui/AboutDialog.ui
        src/ui/ConnectionReselectForDeviceDialog.ui
        src/ui/DeviceChooserDialog.ui
        src/ui/FileAdditionProgressDialog.ui
        src/ui/MainWindow.ui
        src/ui/ManageDevicesDialog.ui
        src/ui/ShowTransferDialog.ui
        src/ui/TransferRequestProgressDialog.ui
        src/dialog/WelcomeDialog.ui)

set(UTIL_LIBRARY
        src/build.h
        src/broadcast/CommunicationServer.h
        src/broadcast/CommunicationServer.cpp
        src/broadcast/DNSSDService.h
        src/broadcast/DNSSDService.cpp
        src/broadcast/SeamlessServer.h
        src/broadcast/SeamlessServer.cpp
        src/broadcast/SeamlessClient.h
        src/broadcast/SeamlessClient.cpp
        src/config/Config.h
        src/config/Config.cpp
        src/config/Keyword.h
        src/database/AccessDatabase.h
        src/database/AccessDatabase.cpp
        src/database/DatabaseLoader.h
        src/database/DatabaseLoader.cpp
        src/database/ReconstructionException.h
        src/database/ReconstructionException.cpp
        src/database/object/NetworkDevice.h
        src/database/object/NetworkDevice.cpp
        src/database/object/TextStreamObject.h
        src/database/object/TextStreamObject.cpp
        src/database/object/TransferObject.h
        src/database/object/TransferObject.cpp
        src/database/object/TransferGroup.h
        src/database/object/TransferGroup.cpp
        src/dialog/WelcomeDialog.h
        src/dialog/WelcomeDialog.cpp
        src/model/ConnectionModel.h
        src/model/ConnectionModel.cpp
        src/model/FlawedTransferModel.h
        src/model/FlawedTransferModel.cpp
        src/model/NetworkDeviceModel.h
        src/model/NetworkDeviceModel.cpp
        src/model/TextStreamModel.h
        src/model/TextStreamModel.cpp
        src/model/TransferGroupModel.h
        src/model/TransferGroupModel.cpp
        src/model/TransferObjectModel.h
        src/model/TransferObjectModel.cpp
        src/ui/AboutDialog.h
        src/ui/AboutDialog.cpp
        src/ui/ConnectionReselectForDeviceDialog.h
        src/ui/ConnectionReselectForDeviceDialog.cpp
        src/ui/DeviceChooserDialog.h
        src/ui/DeviceChooserDialog.cpp
        src/ui/FileAdditionProgressDialog.h
        src/ui/FileAdditionProgressDialog.cpp
        src/ui/MainWindow.h
        src/ui/MainWindow.cpp
        src/ui/ManageDevicesDialog.h
        src/ui/ManageDevicesDialog.cpp
        src/ui/ShowTransferDialog.h
        src/ui/ShowTransferDialog.cpp
        src/ui/TransferRequestProgressDialog.h
        src/ui/TransferRequestProgressDialog.cpp
        src/util/AppUtils.h
        src/util/AppUtils.cpp
        src/util/CommunicationBridge.h
        src/util/CommunicationBridge.cpp
        src/util/GThread.h
        src/util/GThread.cpp
        src/util/Interrupter.h
        src/util/Interrupter.cpp
        src/util/NetworkDeviceLoader.h
        src/util/NetworkDeviceLoader.cpp
        src/util/SynchronizedList.h
        src/util/SynchronizedList.cpp
        src/util/TransferUtils.h
        src/util/TransferUtils.cpp
        src/util/ViewUtils.h
        src/util/ViewUtils.cpp)

add_executable(${PROJECT_EXECUTABLE} src/main.cpp ${UTIL_LIBRARY} ${UI_HEADERS} ${QRCS})

# Add the Qt5 Widgets for linking
if (NOT TARGET Qt5::WinMain)
    add_library(Qt5::WinMain STATIC IMPORTED)
endif ()

if (KF5DNSSD_FOUND)
    add_definitions(-DUSE_DNSSD_FEATURE=1)
    target_link_libraries(${PROJECT_EXECUTABLE} PUBLIC KF5::DNSSD)
else ()
    MESSAGE(STATUS "KDNSSD not found. Choosing qmdnsengine. Do not forget to sync submodules with git")
    add_subdirectory(libs/qmdnsengine)
    target_link_libraries(${PROJECT_EXECUTABLE} PUBLIC libs/qmdnsengine)
endif ()

add_subdirectory(libs/coolsocket)
target_link_libraries(${PROJECT_EXECUTABLE} PUBLIC coolsocket Qt5::Core Qt5::Gui Qt5::Network Qt5::Sql
        Qt5::Widgets)

fix_win_compiler()

if (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_EXECUTABLE}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib)

    install(FILES assets/drawable/${PROJECT_EXECUTABLE}.png
            DESTINATION share/icons/hicolor/256x256/apps/)

    install(FILES dist/${PROJECT_EXECUTABLE}.desktop
            DESTINATION share/applications/)

    install(FILES dist/${PROJECT_EXECUTABLE}.desktop.appdata.xml
            DESTINATION share/appdata/)
elseif (WIN32 AND MSVC)
    make_directory(${LINKED_LIBRARY_DIR})

    configure_file(dist/Windows/package.nsi.in package.nsi)

    install(TARGETS ${PROJECT_EXECUTABLE}
            RUNTIME DESTINATION "/"
            LIBRARY DESTINATION lib)

    add_custom_command(TARGET ${PROJECT_EXECUTABLE} POST_BUILD
            COMMAND ${QT_BINARY_DIR}/windeployqt.exe ${PROJECT_EXECUTABLE}.exe --dir ${LINKED_LIBRARY_DIR})

    add_custom_command(TARGET ${PROJECT_EXECUTABLE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LINKED_LIBRARY_DIR} $<TARGET_FILE_DIR:${PROJECT_EXECUTABLE}>)

    install(DIRECTORY ${LINKED_LIBRARY_DIR}/ DESTINATION .
            PATTERN ${LINKED_LIBRARY_DIR}/*)
else ()
    MESSAGE(FATAL_ERROR "Your host system is currently unsupported. If you want to have it supported, "
            "submit an issue on the tracker on GitHub. If you think you can get it working, feel free "
            "submit your patches.")
endif ()