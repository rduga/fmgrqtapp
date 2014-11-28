TEMPLATE = app
TARGET = FileManager
CONFIG += release
QT += core \
    gui
UI_DIR += src/
HEADERS += src/FMoveDialog.h \
    src/FDeleteDialog.h \
    src/FCopyDialog.h \
    src/FSettings.h \
    src/FImageViewer.h \
    src/FPermissionDialog.h \
    src/FTextEditor.h \
    src/FListPanel.h \
    src/FShortcutButtons.h \
    src/FInclude.h \
    src/FFileManager.h \
    src/FListView.h
SOURCES += src/FListPanel.cpp \
    src/FMoveDialog.cpp \
    src/FDeleteDialog.cpp \
    src/FCopyDialog.cpp \
    src/FSettings.cpp \
    src/FImageViewer.cpp \
    src/FPermissionDialog.cpp \
    src/FTextEditor.cpp \
    src/FShortcutButtons.cpp \
    src/main.cpp \
    src/FFileManager.cpp \
    src/FListView.cpp
FORMS += 
RESOURCES += rsc/general_files.qrc \
    rsc/32x32/icons32.qrc \
    rsc/general_icons.qrc
