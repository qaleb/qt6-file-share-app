#ifndef PLATFORM_H
#define PLATFORM_H

#include <qglobal.h>

class QString;

class Platform
{
public:
    static QString getSystemUsername();
    static QString getHostname();
    static QString getPlatformName();
    static QString getAvatarPath();
    static QString getDefaultPath();
    static QString username;
    static void updateUsername(const QString &name);

    // private:
    Platform() {}
#if defined(Q_OS_UNIX)
    static QString getLinuxAvatarPath();
#elif defined(Q_OS_MAC)
    static QString getMacTempAvatarPath();
#elif defined(Q_OS_WIN)
    static QString getWinTempAvatarPath();
#endif
#if defined(Q_OS_ANDROID)
    static QString getAndroidAvatarPath();
#endif
};

#endif // PLATFORM_H
