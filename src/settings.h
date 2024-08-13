#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QRect>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    QString currentPath();
    void savePath(QString path);
    void saveWindowGeometry(QByteArray geo);
    void saveWindowGeometry(QRect geo);
    QByteArray windowGeometry();
    QRect windowRect();
    void saveThemeColor(QString color);
    QString themeColor();
    void saveShowTermsOnStart(bool show);
    bool showTermsOnStart();
    void saveBuddyName(QString name);
    QString buddyName();

signals:

public slots:

private:
    QSettings mSettings;

};

#endif // SETTINGS_H
