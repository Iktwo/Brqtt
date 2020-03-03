QT += quick

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += $$files($$PWD/src/*.h)

SOURCES += $$files($$PWD/src/*.cpp)

RESOURCES += resources.qrc

TRANSLATIONS += brqtt_en_US.ts

QML_IMPORT_PATH = $$PWD/qml

INCLUDEPATH += "$$PWD/include"

unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
