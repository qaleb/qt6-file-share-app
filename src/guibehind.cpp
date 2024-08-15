#include "guibehind.h"
#include "platform.h"

#include <QDebug>
#include <QQmlContext>
#include <QDir>
#include <QRandomGenerator>

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#endif

// The constructor is private and can only be called within the singleton instance method
GuiBehind::GuiBehind(QQmlApplicationEngine &engine, QObject *parent) :
    QObject(parent), mSettings(this), mDestBuddy(NULL)
{
    // Change current folder
    QDir::setCurrent(mSettings.currentPath());

    // Add "Me" entry
    mBuddiesList.addMeElement();

    // Add "Ip" entry
    mBuddiesList.addIpElement();

    // Destination buddy
    mDestBuddy = new DestinationBuddy(this);

    // Initialize and set the current theme color
    mTheme.setThemeColor(mSettings.themeColor());

    // Expose instances to QML context
    engine.rootContext()->setContextProperty("theme", &mTheme);
    engine.rootContext()->setContextProperty("guiBehind", this);
    engine.rootContext()->setContextProperty("ipAddressesData", &mIpAddresses);
    engine.rootContext()->setContextProperty("buddiesListData", &mBuddiesList);
    engine.rootContext()->setContextProperty("destinationBuddy", mDestBuddy);
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

