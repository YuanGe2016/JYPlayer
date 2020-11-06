TEMPLATE = app
TARGET = MZplayer

QT += widgets multimedia winextras

HEADERS = \
    musicplayer.h \
    volumebutton.h \
    rose.h

SOURCES = \
    main.cpp \
    musicplayer.cpp \
    volumebutton.cpp \
    rose.cpp

RC_ICONS = icon/Love.ICO

target.path = $$[QT_INSTALL_EXAMPLES]/winextras/musicplayer
INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    image/rose_1.png \
    image/rose_2.png \
    image/rose_3.png \
    image/rose_4.png \
    image/rose_5.png
