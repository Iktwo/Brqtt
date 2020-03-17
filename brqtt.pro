QT += quick multimedia

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += $$files($$PWD/src/*.h)

SOURCES += $$files($$PWD/src/*.cpp)

RESOURCES += resources.qrc

TRANSLATIONS += brqtt_en_US.ts

QML_IMPORT_PATH = $$PWD/qml

LIBS += "$$PWD/lib/Cr_Core.lib"

INCLUDEPATH += "$$PWD/include"

unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += README.md

DISTFILES += \
    LICENSE
