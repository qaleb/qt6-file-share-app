#include "guibehind.h"
#include "platform.h"

#include <QDebug>
#include <QQmlContext>
#include <QDir>
#include <QDesktopServices>
#include <QRandomGenerator>
#include <QStandardPaths>

#include <QTimer>
#include <QTime>

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#include <QJniEnvironment>
#endif

#define NETWORK_PORT 4644 // 6742

// The constructor is private and can only be called within the singleton instance method
GuiBehind::GuiBehind(QQmlApplicationEngine &engine, QObject *parent) :
    QObject(parent), mSettings(this), mDestBuddy(NULL), mMiniWebServer(NULL)
{
    // Change current folder
    QDir::setCurrent(mSettings.currentPath());

    QString rootPath = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0);
    qDebug() << "The general download path is:" << rootPath;

    // Add "Me" entry
    mBuddiesList.addMeElement();

    // Add "Ip" entry
    mBuddiesList.addIpElement();

    // Destination buddy
    mDestBuddy = new DestinationBuddy(this);

    // Mini web server
    mMiniWebServer = new MiniWebServer(NETWORK_PORT + 1);

    // Initialize and set the current theme color
    mTheme.setThemeColor(mSettings.themeColor());

    // Expose instances to QML context
    engine.rootContext()->setContextProperty("theme", &mTheme);
    engine.rootContext()->setContextProperty("guiBehind", this);
    engine.rootContext()->setContextProperty("ipAddressesData", &mIpAddresses);
    engine.rootContext()->setContextProperty("buddiesListData", &mBuddiesList);
    engine.rootContext()->setContextProperty("destinationBuddy", mDestBuddy);

    // Register protocol signals
    connect(&mDuktoProtocol, SIGNAL(peerListAdded(Peer)), this, SLOT(peerListAdded(Peer)));
    connect(&mDuktoProtocol, SIGNAL(peerListRemoved(Peer)), this, SLOT(peerListRemoved(Peer)));
    // connect(&mDuktoProtocol, SIGNAL(receiveFileStart(QString)), this, SLOT(receiveFileStart(QString)));
    // connect(&mDuktoProtocol, SIGNAL(transferStatusUpdate(qint64,qint64)), this, SLOT(transferStatusUpdate(qint64,qint64)));
    // connect(&mDuktoProtocol, SIGNAL(receiveFileComplete(QStringList*,qint64)), this, SLOT(receiveFileComplete(QStringList*,qint64)));
    // connect(&mDuktoProtocol, SIGNAL(receiveTextComplete(QString*,qint64)), this, SLOT(receiveTextComplete(QString*,qint64)));
    // connect(&mDuktoProtocol, SIGNAL(sendFileComplete(QStringList*)), this, SLOT(sendFileComplete(QStringList*)));
    // connect(&mDuktoProtocol, SIGNAL(sendFileError(int)), this, SLOT(sendFileError(int)));
    // connect(&mDuktoProtocol, SIGNAL(receiveFileCancelled()), this, SLOT(receiveFileCancelled()));
    // connect(&mDuktoProtocol, SIGNAL(sendFileAborted()), this, SLOT(sendFileAborted()));

    // Register other signals
    // connect(this, SIGNAL(remoteDestinationAddressChanged()), this, SLOT(remoteDestinationAddressHandler()));

    // Say "hello"
    mDuktoProtocol.setPorts(NETWORK_PORT, NETWORK_PORT);
    mDuktoProtocol.initialize();
    mDuktoProtocol.sayHello(QHostAddress::Broadcast);

    // Periodic "hello" timer
    mPeriodicHelloTimer = new QTimer(this);
    // connect(mPeriodicHelloTimer, SIGNAL(timeout()), this, SLOT(periodicHello()));
    mPeriodicHelloTimer->start(60000);
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

QString GuiBehind::currentPath()
{
    return mSettings.currentPath();
}

void GuiBehind::setCurrentPath(QString path)
{
    if (path == mSettings.currentPath()) return;
    mSettings.savePath(path);
    emit currentPathChanged();
}

void GuiBehind::changeDestinationFolder(QString dirpath)
{
    // Convert URL to local file path
    QUrl url(dirpath);
    if (url.isLocalFile()) {
        dirpath = url.toLocalFile();
    }

    if (dirpath.isEmpty()) return;

    // Set the new folder as current
    QDir::setCurrent(dirpath);

    // Save the new setting
    setCurrentPath(dirpath);
}

void GuiBehind::openDestinationFolder()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath()));
}

// Add the new buddy to the buddy list
void GuiBehind::peerListAdded(Peer peer) {
    mBuddiesList.addBuddy(peer);
}

// Remove the buddy from the buddy list
void GuiBehind::peerListRemoved(Peer peer) {

    // Check if currently is shown the "send" page for that buddy
    if (((overlayState() == "send")
         || ((overlayState() == "showtext") && textSnippetSending()))
        && (mDestBuddy->ip() == peer.address.toString()))
        emit hideAllOverlays();

    // Check if currently is shown the "transfer complete" message box
    // for the removed user as destination
    if ((overlayState() == "message") && (messagePageBackState() == "send")
        && (mDestBuddy->ip() == peer.address.toString()))
        setMessagePageBackState("");

    // Remove from the list
    mBuddiesList.removeBuddy(peer.address.toString());
}

void GuiBehind::showRandomBack()
{
    // Look for a random element
    int i = QRandomGenerator::global()->bounded(mBuddiesList.rowCount());

    // Show back
    if (i < mBuddiesList.rowCount()) mBuddiesList.showSingleBack(i);
}

void GuiBehind::setBuddyName(QString name)
{
    qDebug() << "Buddy name is:  " << name;
    mSettings.saveBuddyName(name.replace(' ', ""));
    mDuktoProtocol.updateBuddyName();
    mBuddiesList.updateMeElement();
    emit buddyNameChanged();
}

QRect GuiBehind::windowGeometry()
{
    return mSettings.windowRect();
}

void GuiBehind::setWindowGeometry(QRect geo)
{
    mSettings.saveWindowGeometry(geo);
}

QString GuiBehind::buddyName()
{
    return mSettings.buddyName();
}

bool GuiBehind::textSnippetSending()
{
    return mTextSnippetSending;
}

void GuiBehind::setTextSnippetSending(bool sending)
{
    if (sending == mTextSnippetSending) return;
    mTextSnippetSending = sending;
    emit textSnippetSendingChanged();
}

QString GuiBehind::overlayState()
{
    return mOverlayState;
}

void GuiBehind::setOverlayState(QString state)
{
    if (state == mOverlayState) return;
    mOverlayState = state;
    emit overlayStateChanged();
}

QString GuiBehind::messagePageBackState()
{
    return mMessagePageBackState;
}

void GuiBehind::setMessagePageBackState(QString state)
{
    if (state == mMessagePageBackState) return;
    mMessagePageBackState = state;
    emit messagePageBackStateChanged();
}
