TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


LIBS += -lwiringPi

SOURCES += \
        bme280.c \
        main.c

HEADERS += \
    bme280.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/elhem
!isEmpty(target.path): INSTALLS += target

