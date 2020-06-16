QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    components/custom_window.cpp \
    components/layer_select_button.cpp \
    components/overlay_widget.cpp \
    components/panel.cpp \
    components/super_video_widget.cpp \
    components/timeline_widget.cpp \
    layer.cpp \
    main.cpp \
    windows/editor_window.cpp

HEADERS += \
    components/better_line_edit.h \
    components/custom_window.h \
    components/layer_select_button.h \
    components/overlay_widget.h \
    components/panel.h \
    components/super_video_widget.h \
    components/timeline_widget.h \
    help.h \
    layer.h \
    segment.h \
    windows/editor_window.h

FORMS +=

TRANSLATIONS += \
    EyeBang_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
