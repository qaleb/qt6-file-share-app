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
    // Provide a static method to access the singleton instance
    static GuiBehind& instance(QQmlApplicationEngine &engine) {
        static GuiBehind instance(engine);
        return instance;
    }

    Q_INVOKABLE QString getCurrentThemeColor();

public slots:
    void changeThemeColor(QString color);
    void refreshIpList();

private:
    // Make constructor private to prevent direct instantiation
    explicit GuiBehind(QQmlApplicationEngine &engine, QObject *parent = nullptr);
    GuiBehind(const GuiBehind&) = delete; // Delete copy constructor
    GuiBehind& operator=(const GuiBehind&) = delete; // Delete assignment operator

    Settings mSettings;
    Theme mTheme;
    IpAddressItemModel mIpAddresses;
};

#endif // GUIBEHIND_H
