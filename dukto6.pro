QT += quick
QT += core5compat
QT += widgets
QT += core
Qt += core-private

VERSION = 6.2024.01.01
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    src/buddylistitemmodel.cpp \
    src/destinationbuddy.cpp \
    src/duktoprotocol.cpp \
    src/guibehind.cpp \
    src/ipaddressitemmodel.cpp \
    src/main.cpp \
    src/miniwebserver.cpp \
    src/platform.cpp \
    src/recentlistitemmodel.cpp \
    src/settings.cpp \
    src/theme.cpp \
    src/updateschecker.cpp

RESOURCES += \
    src/assets.qrc \
    src/qml.qrc

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

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    src/assets/icons/MoreIcon.svg

HEADERS += \
    src/buddylistitemmodel.h \
    src/destinationbuddy.h \
    src/duktoprotocol.h \
    src/guibehind.h \
    src/ipaddressitemmodel.h \
    src/miniwebserver.h \
    src/peer.h \
    src/platform.h \
    src/recentlistitemmodel.h \
    src/settings.h \
    src/theme.h \
    src/updateschecker.h

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
