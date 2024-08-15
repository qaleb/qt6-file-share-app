#include "settings.h"
#include "platform.h"

#include <QSettings>
#include <QDir>
#include "theme.h"

#include <QDebug>

Settings::Settings(QObject *parent) :
    QObject(parent), mSettings("dukto", "Dukto")
{
    // Ensure the default theme color is set if it's not already in settings
    if (mSettings.value("ThemeColor").toString().isEmpty()) {
        mSettings.setValue("ThemeColor", Theme::DEFAULT_THEME_COLOR);
    }
}

QString Settings::currentPath()
{
    // Retrieve the last saved path (if any)
    QString path = mSettings.value("DestPath", "").toString();
    // qDebug() << "The current path is:" << path;
    // Check if exists
    if ((path != "") && (QDir(path).exists()))
        return path;

    // Else return the default path for this platform
    path = Platform::getDefaultPath();
    if (QDir(path).exists())
        return path;
    else
        return ".";
}

void Settings::savePath(QString path)
{
    // Save the new path
    mSettings.setValue("DestPath", path);
    mSettings.sync();
}

void Settings::saveWindowGeometry(QByteArray geo)
{
    mSettings.setValue("WindowPosAndSize", geo);
    mSettings.sync();
}

void Settings::saveWindowGeometry(QRect geo)
{
    // QRect staticRect(geo.left(), geo.top(), 360, 600);
    mSettings.setValue("WindowPosAndSize", geo);
    mSettings.sync();
}

QByteArray Settings::windowGeometry()
{
    return mSettings.value("WindowPosAndSize").toByteArray();
}

QRect Settings::windowRect()
{
    return mSettings.value("WindowPosAndSize", QRect(20,40,360,600)).toRect();
}

void Settings::saveThemeColor(QString color)
{
    mSettings.setValue("ThemeColor", color);
    mSettings.sync();
}

QString Settings::themeColor()
{
    return mSettings.value("ThemeColor", Theme::DEFAULT_THEME_COLOR).toString();
}

void Settings::saveShowTermsOnStart(bool show)
{
    mSettings.setValue("R6/ShowTermsOnStart", show);
    mSettings.sync();
}

bool Settings::showTermsOnStart()
{
    return mSettings.value("R6/ShowTermsOnStart", true).toBool();
}

QString Settings::buddyName()
{
    // Retrieve the last saved name (if any)
    return mSettings.value("BuddyName", "").toString();
}

void Settings::saveBuddyName(QString name)
{
    // Save the new name
    mSettings.setValue("BuddyName", name);
    qDebug() << "The buddyName to be saved is:" << name;
    mSettings.sync();

    // Update the static username when the buddyName is set
    Platform::updateUsername(name);
}
