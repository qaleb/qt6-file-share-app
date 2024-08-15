#ifndef GUIBEHIND_H
#define GUIBEHIND_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "theme.h"
#include "settings.h"
#include "ipaddressitemmodel.h"
#include "buddylistitemmodel.h"
#include "destinationbuddy.h"
#include "duktoprotocol.h"

class GuiBehind : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged FINAL)
    Q_PROPERTY(QString overlayState READ overlayState WRITE setOverlayState NOTIFY overlayStateChanged FINAL)
    Q_PROPERTY(QString buddyName READ buddyName WRITE setBuddyName NOTIFY buddyNameChanged FINAL)
    Q_PROPERTY(bool textSnippetSending READ textSnippetSending WRITE setTextSnippetSending NOTIFY textSnippetSendingChanged FINAL)
    Q_PROPERTY(QString messagePageBackState READ messagePageBackState WRITE setMessagePageBackState NOTIFY messagePageBackStateChanged FINAL)

public:
    // Provide a static method to access the singleton instance
    static GuiBehind& instance(QQmlApplicationEngine &engine) {
        static GuiBehind instance(engine);
        return instance;
    }

    QString getCurrentThemeColor();
    QString currentPath();
    void setCurrentPath(QString path);
    QString buddyName();
    QString overlayState();
    void setOverlayState(QString state);
    bool textSnippetSending();
    void setTextSnippetSending(bool sending);
    QString messagePageBackState();
    void setMessagePageBackState(QString state);

signals:
    void currentPathChanged();
    void buddyNameChanged();
    void textSnippetSendingChanged();
    void messagePageBackStateChanged();
    void overlayStateChanged();

    void hideAllOverlays();

public slots:
    // Called by QML
    void changeThemeColor(QString color);
    void refreshIpList();
    void changeDestinationFolder(QString dirname);
    void peerListAdded(Peer peer);
    void peerListRemoved(Peer peer);
    void showRandomBack();
    void setBuddyName(QString name);
    QRect windowGeometry();
    void setWindowGeometry(QRect geo);

private:
    // Make constructor private to prevent direct instantiation
    explicit GuiBehind(QQmlApplicationEngine &engine, QObject *parent = nullptr);
    GuiBehind(const GuiBehind&) = delete; // Delete copy constructor
    GuiBehind& operator=(const GuiBehind&) = delete; // Delete assignment operator

    Settings mSettings;
    Theme mTheme;
    IpAddressItemModel mIpAddresses;
    DestinationBuddy *mDestBuddy;
    BuddyListItemModel mBuddiesList;
    DuktoProtocol mDuktoProtocol;

    bool mTextSnippetSending;
    QString mMessagePageBackState;
    QString mOverlayState;
};

#endif // GUIBEHIND_H
