QT -= gui
QT += network

CONFIG += c++14 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        basicaisnake.cpp \
        coordinates.cpp \
        field.cpp \
        foodmanager.cpp \
        game.cpp \
        main.cpp \
        network/clientprocessor.cpp \
        network/gamemessage.cpp \
        network/server.cpp \
        snake.cpp \
        snakecontroller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    basicaisnake.h \
    coordinates.h \
    field.h \
    foodmanager.h \
    game.h \
    network/clientprocessor.h \
    network/gamemessage.h \
    network/headers.h \
    network/server.h \
    snake.h \
    snakecontroller.h
