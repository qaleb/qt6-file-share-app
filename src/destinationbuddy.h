#ifndef DESTINATIONBUDDY_H
#define DESTINATIONBUDDY_H

#include <QObject>

class QStandardItem;

class DestinationBuddy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ip READ ip NOTIFY ipChanged)
    Q_PROPERTY(qint16 port READ port NOTIFY portChanged)
    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)
    Q_PROPERTY(QString system READ system NOTIFY systemChanged)
    Q_PROPERTY(QString platform READ platform NOTIFY platformChanged)
    Q_PROPERTY(QString genericAvatar READ genericAvatar NOTIFY genericAvatarChanged)
    Q_PROPERTY(QString avatar READ avatar NOTIFY avatarChanged)
    Q_PROPERTY(QString osLogo READ osLogo NOTIFY osLogoChanged)
    Q_PROPERTY(QString showBack READ showBack NOTIFY showBackChanged)

public:
    explicit DestinationBuddy(QObject *parent = 0);
    inline QString ip() { return mIp; }
    inline qint16 port() { return mPort; }
    inline QString username() { return mUsername; }
    inline QString system() { return mSystem; }
    inline QString platform() { return mPlatform; }
    inline QString genericAvatar() { return mGenericAvatar; }
    inline QString avatar() { return mAvatar; }
    inline QString osLogo() { return mOsLogo; }
    inline QString showBack() { return mShowBack; }
    void fillFromItem(QStandardItem *item);
    // void setAsRemoteBuddy(QString ip);

signals:
    void ipChanged();
    void portChanged();
    void usernameChanged();
    void systemChanged();
    void platformChanged();
    void genericAvatarChanged();
    void avatarChanged();
    void osLogoChanged();
    void showBackChanged();

public slots:

private:
    QString mIp;
    qint16 mPort;
    QString mUsername;
    QString mSystem;
    QString mPlatform;
    QString mGenericAvatar;
    QString mAvatar;
    QString mOsLogo;
    QString mShowBack;
};

#endif // DESTINATIONBUDDY_H
