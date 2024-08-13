#include "guibehind.h"
#include "platform.h"
#include <QDebug>
#include <QQmlContext>

// The constructor is private and can only be called within the singleton instance method
GuiBehind::GuiBehind(QQmlApplicationEngine &engine, QObject *parent) :
    QObject(parent), mSettings(this)
{
    // Initialize and set the current theme color
    mTheme.setThemeColor(mSettings.themeColor());

    // Expose instances to QML context
    engine.rootContext()->setContextProperty("theme", &mTheme);
    engine.rootContext()->setContextProperty("guiBehind", this);
    engine.rootContext()->setContextProperty("ipAddressesData", &mIpAddresses);
}

void GuiBehind::changeThemeColor(QString color)
{
    mTheme.setThemeColor(color);
    mSettings.saveThemeColor(color);
}

QString GuiBehind::getCurrentThemeColor()
{
    return mSettings.themeColor();
}

void GuiBehind::refreshIpList()
{
    mIpAddresses.refreshIpList();
}
