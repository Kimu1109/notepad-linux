QT       += core gui
QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutnotepad.cpp \
    finddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    replacedialog.cpp \
    saveformatdialog.cpp \
    zoomableplaintextedit.cpp

HEADERS += \
    aboutnotepad.h \
    character_controller.h \
    finddialog.h \
    mainwindow.h \
    replacedialog.h \
    saveformatdialog.h \
    zoomableplaintextedit.h

FORMS += \
    aboutnotepad.ui \
    finddialog.ui \
    mainwindow.ui \
    replacedialog.ui \
    saveformatdialog.ui

unix {
    LIBS += -licuuc -licui18n -licudata
}

win32 {
    # vcpkg などでインストールした ICU のパス
    ICU_PATH = C:/vcpkg/installed/x64-windows
    INCLUDEPATH += $$ICU_PATH/include
    LIBS += $$ICU_PATH/lib/icuuc.lib
    LIBS += $$ICU_PATH/lib/icuin.lib
    LIBS += $$ICU_PATH/lib/icudt.lib
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
