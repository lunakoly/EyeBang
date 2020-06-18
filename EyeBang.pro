QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/better_line_edit.cpp \
    components/layer_select_button.cpp \
    components/overlay.cpp \
    components/panel.cpp \
    components/placeholder.cpp \
    components/timeline.cpp \
    components/video_tab.cpp \
    editing/layer.cpp \
    editing/project.cpp \
    main.cpp \
    windows/custom_window.cpp \
    windows/editor_window_2.cpp \
    windows/overlay_window.cpp

HEADERS += \
    aliases.h \
    components/better_line_edit.h \
    components/layer_select_button.h \
    components/overlay.h \
    components/panel.h \
    components/placeholder.h \
    components/timeline.h \
    components/video_tab.h \
    editing/layer.h \
    editing/project.h \
    editing/segment.h \
    help.h \
    windows/custom_window.h \
    windows/editor_window_2.h \
    windows/overlay_window.h

FORMS +=

TRANSLATIONS += \
    EyeBang_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
