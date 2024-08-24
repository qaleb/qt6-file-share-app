#ifndef GUIBEHIND_H
#define GUIBEHIND_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QClipboard>

#if defined(Q_OS_S60)
#include <QNetworkSession>
#endif

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


#include "buddylistitemmodel.h"
#include "recentlistitemmodel.h"
#include "ipaddressitemmodel.h"
#include "destinationbuddy.h"
#include "duktoprotocol.h"
#include "theme.h"
#include "settings.h"
#include "miniwebserver.h"
#include "updateschecker.h"

class MiniWebServer;
class QNetworkAccessManager;
class QNetworkReply;

class GuiBehind : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentTransferProgress READ currentTransferProgress NOTIFY currentTransferProgressChanged)
    Q_PROPERTY(QString currentTransferStats READ currentTransferStats NOTIFY currentTransferStatsChanged)
    Q_PROPERTY(bool currentTransferSending READ currentTransferSending NOTIFY currentTransferSendingChanged)
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged FINAL)
    Q_PROPERTY(QString overlayState READ overlayState WRITE setOverlayState NOTIFY overlayStateChanged FINAL)
    Q_PROPERTY(QString buddyName READ buddyName WRITE setBuddyName NOTIFY buddyNameChanged FINAL)
    Q_PROPERTY(QString currentTransferBuddy READ currentTransferBuddy WRITE setCurrentTransferBuddy NOTIFY currentTransferBuddyChanged FINAL)
    Q_PROPERTY(QString textSnippetBuddy READ textSnippetBuddy NOTIFY textSnippetBuddyChanged)
    Q_PROPERTY(QString textSnippet READ textSnippet WRITE setTextSnippet NOTIFY textSnippetChanged)
    Q_PROPERTY(bool textSnippetSending READ textSnippetSending WRITE setTextSnippetSending NOTIFY textSnippetSendingChanged FINAL)
    Q_PROPERTY(QString messagePageBackState READ messagePageBackState WRITE setMessagePageBackState NOTIFY messagePageBackStateChanged FINAL)
    Q_PROPERTY(QString remoteDestinationAddress READ remoteDestinationAddress WRITE setRemoteDestinationAddress NOTIFY remoteDestinationAddressChanged FINAL)
    Q_PROPERTY(QString messagePageText READ messagePageText WRITE setMessagePageText NOTIFY messagePageTextChanged)
    Q_PROPERTY(QString messagePageTitle READ messagePageTitle WRITE setMessagePageTitle NOTIFY messagePageTitleChanged)
    Q_PROPERTY(bool showTermsOnStart READ showTermsOnStart WRITE setShowTermsOnStart NOTIFY showTermsOnStartChanged)
    Q_PROPERTY(bool showUpdateBanner READ showUpdateBanner WRITE setShowUpdateBanner NOTIFY showUpdateBannerChanged)
    Q_PROPERTY(bool clipboardTextAvailable READ clipboardTextAvailable NOTIFY clipboardTextAvailableChanged)
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)

public:
    // Provide a static method to access the singleton instance
    static GuiBehind& instance(QQmlApplicationEngine &engine) {
        static GuiBehind instance(engine);
        return instance;
    }

    QString getCurrentThemeColor();
    // bool canAcceptDrop();
    // void sendDroppedFiles(QStringList *files);

    QString currentTransferBuddy();
    void setCurrentTransferBuddy(QString buddy);
    int currentTransferProgress();
    void setCurrentTransferProgress(int value);
    QString currentTransferStats();
    void setCurrentTransferStats(QString stats);
    QString textSnippetBuddy();
    void setTextSnippetBuddy(QString buddy);
    QString textSnippet();
    void setTextSnippet(QString txt);
    bool textSnippetSending();
    void setTextSnippetSending(bool sending);
    QString currentPath();
    void setCurrentPath(QString path);
    //    QString currentUsername();
    //    void setCurrentUsername(QString username);
    bool currentTransferSending();
    void setCurrentTransferSending(bool sending);
    bool clipboardTextAvailable();
    QString remoteDestinationAddress();
    void setRemoteDestinationAddress(QString address);
    QString overlayState();
    void setOverlayState(QString state);
    QString messagePageText();
    void setMessagePageText(QString message);
    QString messagePageTitle();
    void setMessagePageTitle(QString title);
    QString messagePageBackState();
    void setMessagePageBackState(QString state);
    bool showTermsOnStart();
    void setShowTermsOnStart(bool show);
    bool showUpdateBanner();
    void setShowUpdateBanner(bool show);
    //    void setBuddyName(QString name);


    QString buddyName();
    QString appVersion();

    bool isTrayIconVisible();
    void setTrayIconVisible(bool bVisible);

    #if defined(Q_OS_ANDROID)
    // Convert content URI to local file paths
    QString convertContentUriToFilePath(const QString &files);
    // Request Permissions on Android
    bool requestPermissions();
    #endif

#if defined(Q_OS_S60)
    void initConnection();
#endif

signals:
    void currentTransferBuddyChanged();
    void currentTransferProgressChanged();
    void currentTransferStatsChanged();
    void currentTransferSendingChanged();
    void textSnippetBuddyChanged();
    void textSnippetChanged();
    void textSnippetSendingChanged();
    void currentPathChanged();
    //    void currentUsernameChanged();
    void clipboardTextAvailableChanged();
    void remoteDestinationAddressChanged();
    void overlayStateChanged();
    void messagePageTextChanged();
    void messagePageTitleChanged();
    void messagePageBackStateChanged();
    void showTermsOnStartChanged();
    void showUpdateBannerChanged();
    void buddyNameChanged();

    // Received by QML
    void transferStart();
    void receiveCompleted();
    void gotoTextSnippet();
    void gotoSendPage();
    void gotoMessagePage();
    void hideAllOverlays();

public slots:
    QRect windowGeometry();
    void setWindowGeometry(QRect geo);

    void showRandomBack();
    void clipboardChanged();
    void remoteDestinationAddressHandler();
    void periodicHello();
    void showUpdatesMessage();
    void sendScreenStage2();

    // Called by Dukto protocol
    void peerListAdded(Peer peer);
    void peerListRemoved(Peer peer);
    void receiveFileStart(QString senderIp);
    void transferStatusUpdate(qint64 total, qint64 partial);
    void receiveFileComplete(QStringList *files, qint64 totalSize);
    void receiveTextComplete(QString *text, qint64 totalSize);
    void sendFileComplete();
    void sendFileError(int code);
    void receiveFileCancelled();
    void sendFileAborted();

    // Called by QML
    void close();
    void openDestinationFolder();
    void refreshIpList();
    void showTextSnippet(QString text, QString sender);
    void openFile(QString path);
    void changeDestinationFolder(QString dirpath);
    void showSendPage(QString ip);
    void sendSomeFiles(const QStringList &files);
    void sendAllFiles(const QStringList &files);
    void sendClipboardText();
    void sendText();
    void sendScreen();
    void changeThemeColor(QString color);
    void setBuddyName(QString name);
    void resetProgressStatus();
    void abortTransfer();
    void sendBuddyDroppedFiles(const QStringList &files);
    bool canAcceptDrop();
    void sendDroppedFiles(const QStringList &files);

#if defined(Q_OS_S60)
    void connectOpened();
    void connectError(QNetworkSession::SessionError error);
#endif

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    // Make constructor private to prevent direct instantiation
    explicit GuiBehind(QQmlApplicationEngine &engine, QObject *parent = nullptr);
    GuiBehind(const GuiBehind&) = delete; // Delete copy constructor
    GuiBehind& operator=(const GuiBehind&) = delete; // Delete assignment operator

    QTimer *mShowBackTimer;
    QTimer *mPeriodicHelloTimer;
    QClipboard *mClipboard;
    MiniWebServer *mMiniWebServer;
    Settings mSettings;
    DestinationBuddy *mDestBuddy;
    BuddyListItemModel mBuddiesList;
    RecentListItemModel mRecentList;
    IpAddressItemModel mIpAddresses;
    DuktoProtocol mDuktoProtocol;
    Theme mTheme;
    UpdatesChecker *mUpdatesChecker;

    int mCurrentTransferProgress;
    QString mCurrentTransferBuddy;
    QString mCurrentTransferStats;
    bool mCurrentTransferSending;
    QString mTextSnippetBuddy;
    QString mTextSnippet;
    bool mTextSnippetSending;
    bool mClipboardTextAvailable;
    QString mRemoteDestinationAddress;
    QString mOverlayState;
    QString mMessagePageText;
    QString mMessagePageTitle;
    QString mMessagePageBackState;
    bool mShowUpdateBanner;
    QString mScreenTempPath;

    bool prepareStartTransfer(QString *ip, qint16 *port);
    void startTransfer(QStringList files);
    void startTransfer(QString text);

    //system tray
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    void createActions();
    void createTrayIcon();
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // GUIBEHIND_H
