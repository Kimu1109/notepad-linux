cmake_minimum_required(VERSION 3.16)

project(notepad VERSION 1.0 LANGUAGES CXX)

# ---- Qt auto tools ----
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---- Qt ----
find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport LinguistTools)

# ---- Executable ----
qt_add_executable(notepad WIN32 MACOSX_BUNDLE
    aboutnotepad.cpp aboutnotepad.h aboutnotepad.ui
    character_controller.h
    finddialog.cpp finddialog.h finddialog.ui
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    replacedialog.cpp replacedialog.h replacedialog.ui
    saveformatdialog.cpp saveformatdialog.h saveformatdialog.ui
    zoomableplaintextedit.cpp zoomableplaintextedit.h
    savedialognotnative.h resources.qrc
    welcomedialog.h welcomedialog.cpp welcomedialog.ui magic_enum.hpp
)

target_link_libraries(notepad PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
)

# ---- ICU ----
find_package(ICU REQUIRED COMPONENTS i18n uc data)
target_link_libraries(notepad PRIVATE
    ICU::i18n
    ICU::uc
    ICU::data
)

qt_add_translations(notepad
    TS_FILES
        translations/app_ja.ts
)

# ---- Install ----
install(TARGETS notepad
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- Qt deploy ----
qt_generate_deploy_app_script(
    TARGET notepad
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

