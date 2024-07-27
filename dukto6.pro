QT += quick

SOURCES += \
        src/main.cpp

resources.files = src/qml/main.qml
resources.prefix = /$${TARGET}
RESOURCES += resources \
    src/resource.qrc

TRANSLATIONS += \
    src/language/dukto6_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
