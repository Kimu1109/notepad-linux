cmake_minimum_required(VERSION 3.16)

# ==============================
# Project
# ==============================
project(notepad
    VERSION 1.0.0
    LANGUAGES CXX
)

# ==============================
# Basic settings
# ==============================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# ==============================
# Qt auto tools
# ==============================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ==============================
# Qt
# ==============================
find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    PrintSupport
    LinguistTools
)

qt_standard_project_setup()

# ==============================
# Executable (Linux専用)
# ==============================
qt_add_executable(notepad
    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    aboutnotepad.cpp
    aboutnotepad.h
    aboutnotepad.ui

    finddialog.cpp
    finddialog.h
    finddialog.ui

    replacedialog.cpp
    replacedialog.h
    replacedialog.ui

    saveformatdialog.cpp
    saveformatdialog.h
    saveformatdialog.ui

    zoomableplaintextedit.cpp
    zoomableplaintextedit.h

    welcomedialog.cpp
    welcomedialog.h
    welcomedialog.ui

    savedialognotnative.h
    character_controller.h
    magic_enum.hpp

    resources.qrc
)

# ==============================
# Link libraries
# ==============================
target_link_libraries(notepad PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
)

# ==============================
# ICU
# ==============================
find_package(ICU REQUIRED COMPONENTS i18n uc data)

target_link_libraries(notepad PRIVATE
    ICU::i18n
    ICU::uc
    ICU::data
)

# ==============================
# Translations
# ==============================
qt_add_translations(notepad
    TS_FILES
        translations/app_ja.ts
)

# ==============================
# Install (for deb)
# ==============================
install(TARGETS notepad
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ==============================
# Desktop entry
# ==============================
install(FILES notepad_linux.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

# ==============================
# Icon
# ==============================
install(FILES icon/512.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps
)
install(FILES icon/256.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps
)
install(FILES icon/128.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/apps
)
install(FILES icon/96.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/96x96/apps
)
install(FILES icon/64.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/64x64/apps
)
install(FILES icon/48.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/48x48/apps
)
install(FILES icon/32.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps
)
install(FILES icon/24.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/24x24/apps
)
install(FILES icon/16.png
    RENAME notepad.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/16x16/apps
)

# ==============================
# CPack (DEB)
# ==============================
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "notepad")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Notepad For Linux")
set(CPACK_PACKAGE_VENDOR "Kimu1109")
set(CPACK_PACKAGE_CONTACT "https://k-studio.ddo.jp")

# ---- Debian specific ----
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kimu1109")
set(CPACK_DEBIAN_PACKAGE_SECTION "editors")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# Qt / ICU 依存（必要に応じて調整）
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libqt6widgets6, libqt6gui6, libqt6core6, libqt6printsupport6, libicu-dev"
)

include(CPack)
