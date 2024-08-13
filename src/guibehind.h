#ifndef GUIBEHIND_H
#define GUIBEHIND_H

#include <QObject>
#include <QQmlApplicationEngine>
#include "theme.h"
#include "settings.h"
#include "ipaddressitemmodel.h"

class GuiBehind : public QObject
{
    Q_OBJECT
public:
    explicit GuiBehind(QQmlApplicationEngine &engine, QObject *parent = nullptr);

    Q_INVOKABLE QString getCurrentThemeColor();

public slots:
    void changeThemeColor(QString color);
    void refreshIpList();

private:
    Settings mSettings;
    Theme mTheme;
    IpAddressItemModel mIpAddresses;
};

#endif // GUIBEHIND_H
