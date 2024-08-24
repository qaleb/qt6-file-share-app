#include "guibehind.h"
#include "platform.h"

#include <QDebug>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlProperty>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QThread>
#include <QTemporaryFile>
#include <QTimer>
#include <QTime>
#include <QClipboard>
#include <QFileDialog>
#include <QApplication>

#include <QGuiApplication>

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#include <QJniEnvironment>
#include <QtCore/private/qandroidextras_p.h>
#include <jni.h>
#include <QtCore/QCoreApplication>
// #include <sharedstorage.h>
// #include "utils.h"
#endif

#define NETWORK_PORT 4644 // 6742

// The constructor is private and can only be called within the singleton instance method
GuiBehind::GuiBehind(QQmlApplicationEngine &engine, QObject *parent) :
    QObject(parent), mShowBackTimer(NULL), mPeriodicHelloTimer(NULL), mClipboard(NULL),
    mMiniWebServer(NULL), mSettings(this), mDestBuddy(NULL), mUpdatesChecker(NULL)
{
    // Change current folder
    QDir::setCurrent(currentPath());

    // QString rootPath = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0);
    // qDebug() << "The general download path is:" << rootPath;

    // Status variables
    // mView->setGuiBehindReference(this);
    setCurrentTransferProgress(0);
    setTextSnippetSending(false);
    setShowUpdateBanner(false);

    // Clipboard object
    mClipboard = QApplication::clipboard();
    connect(mClipboard, SIGNAL(dataChanged()), this, SLOT(clipboardChanged()));
    clipboardChanged(); // Initial call to handle current clipboard data

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
    engine.rootContext()->setContextProperty("buddiesListData", &mBuddiesList);
    engine.rootContext()->setContextProperty("recentListData", &mRecentList);
    engine.rootContext()->setContextProperty("ipAddressesData", &mIpAddresses);
    engine.rootContext()->setContextProperty("guiBehind", this);
    engine.rootContext()->setContextProperty("destinationBuddy", mDestBuddy);
    engine.rootContext()->setContextProperty("theme", &mTheme);

    // Register protocol signals
    connect(&mDuktoProtocol, SIGNAL(peerListAdded(Peer)), this, SLOT(peerListAdded(Peer)));
    connect(&mDuktoProtocol, SIGNAL(peerListRemoved(Peer)), this, SLOT(peerListRemoved(Peer)));
    connect(&mDuktoProtocol, SIGNAL(receiveFileStart(QString)), this, SLOT(receiveFileStart(QString)));
    connect(&mDuktoProtocol, SIGNAL(transferStatusUpdate(qint64,qint64)), this, SLOT(transferStatusUpdate(qint64,qint64)));
    connect(&mDuktoProtocol, SIGNAL(receiveFileComplete(QStringList*,qint64)), this, SLOT(receiveFileComplete(QStringList*,qint64)));
    connect(&mDuktoProtocol, SIGNAL(receiveTextComplete(QString*,qint64)), this, SLOT(receiveTextComplete(QString*,qint64)));
    connect(&mDuktoProtocol, SIGNAL(sendFileComplete(QStringList*)), this, SLOT(sendFileComplete(QStringList*)));
    connect(&mDuktoProtocol, SIGNAL(sendFileError(int)), this, SLOT(sendFileError(int)));
    connect(&mDuktoProtocol, SIGNAL(receiveFileCancelled()), this, SLOT(receiveFileCancelled()));
    connect(&mDuktoProtocol, SIGNAL(sendFileAborted()), this, SLOT(sendFileAborted()));

    // Register other signals
    connect(this, SIGNAL(remoteDestinationAddressChanged()), this, SLOT(remoteDestinationAddressHandler()));

    // Say "hello"
    mDuktoProtocol.setPorts(NETWORK_PORT, NETWORK_PORT);
    mDuktoProtocol.initialize();
    mDuktoProtocol.sayHello(QHostAddress::Broadcast);

    // Periodic "hello" timer
    mPeriodicHelloTimer = new QTimer(this);
    connect(mPeriodicHelloTimer, SIGNAL(timeout()), this, SLOT(periodicHello()));
    mPeriodicHelloTimer->start(60000);

    // Start random rotate
    mShowBackTimer = new QTimer(this);
    connect(mShowBackTimer, SIGNAL(timeout()), this, SLOT(showRandomBack()));
    uint iSeed = QDateTime::currentDateTime().toSecsSinceEpoch();
    srand(iSeed);
    mShowBackTimer->start(10000);

    // Enqueue check for updates
    mUpdatesChecker = new UpdatesChecker();
    connect(mUpdatesChecker, SIGNAL(updatesAvailable()), this, SLOT(showUpdatesMessage()));
    QTimer::singleShot(2000, mUpdatesChecker, SLOT(start()));

#if defined(Q_OS_ANDROID)
    // Request Permissions on Android
    requestPermissions();
#endif
}

#if defined(Q_OS_ANDROID)
// Request Permissions on Android
bool GuiBehind::requestPermissions() {
    QList<bool> permissions;

    // Declare required permissions
    const QStringList permissionList = {
        "android.permission.MANAGE_EXTERNAL_STORAGE",
        "android.permission.WRITE_EXTERNAL_STORAGE",
        "android.permission.WRITE_SETTINGS",
        "android.permission.VIBRATE",
        "android.permission.INTERNET",
        "android.permission.WAKE_LOCK",
        "android.permission.READ_EXTERNAL_STORAGE",
        "android.permission.READ_PROFILE",
        "android.permission.CHANGE_WIFI_MULTICAST_STATE",
        "android.permission.READ_MEDIA_IMAGES",
        "android.permission.READ_MEDIA_VIDEO",
        "android.permission.READ_MEDIA_AUDIO"
    };

    for (const QString &permission : permissionList) {
        auto result = QtAndroidPrivate::checkPermission(permission).result();
        if (result != QtAndroidPrivate::Authorized) {
            result = QtAndroidPrivate::requestPermission(permission).result();
            if (result == QtAndroidPrivate::Denied) {
                permissions.append(false);
            }
        }
    }

    return permissions.isEmpty();
}
#endif

QRect GuiBehind::windowGeometry()
{
    return mSettings.windowRect();
}

void GuiBehind::setWindowGeometry(QRect geo)
{
    mSettings.saveWindowGeometry(geo);
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

void GuiBehind::clipboardChanged()
{
    mClipboardTextAvailable = (mClipboard->text() != "");
    emit clipboardTextAvailableChanged();
}

void GuiBehind::receiveFileStart(QString senderIp)
{
    // Look for the sender in the buddy list
    QString sender = mBuddiesList.buddyNameByIp(senderIp);
    if (sender == "")
        setCurrentTransferBuddy("remote sender");
    else
        setCurrentTransferBuddy(sender);

    // Update user interface
    setCurrentTransferSending(false);
    // mView->win7()->setProgressValue(0, 100);
    // mView->win7()->setProgressState(EcWin7::Normal);

    emit transferStart();
}

void GuiBehind::transferStatusUpdate(qint64 total, qint64 partial)
{
    // Stats formatting
    if (total < 1024)
        setCurrentTransferStats(QString::number(partial) + " B of " + QString::number(total) + " B");
    else if (total < 1048576)
        setCurrentTransferStats(QString::number(partial * 1.0 / 1024, 'f', 1) + " KB of " + QString::number(total * 1.0 / 1024, 'f', 1) + " KB");
    else
        setCurrentTransferStats(QString::number(partial * 1.0 / 1048576, 'f', 1) + " MB of " + QString::number(total * 1.0 / 1048576, 'f', 1) + " MB");

    double percent = partial * 1.0 / total * 100;
    setCurrentTransferProgress(percent);

    // mView->win7()->setProgressValue(percent, 100);
}

void GuiBehind::receiveFileComplete(QStringList *files, qint64 totalSize) {

    // Add an entry to recent activities
    QDir d(".");
    if (files->size() == 1)
        mRecentList.addRecent(files->at(0), d.absoluteFilePath(files->at(0)), "file", mCurrentTransferBuddy, totalSize);
    else
        mRecentList.addRecent(tr("Files and folders"), d.absolutePath(), "misc", mCurrentTransferBuddy, totalSize);

// Update GUI
// mView->win7()->setProgressState(EcWin7::NoProgress);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    //TODO: check QtQuick 2 alert
    QApplication::alert(mView, 5000);
#endif
    emit receiveCompleted();
}

void GuiBehind::receiveTextComplete(QString *text, qint64 totalSize)
{
    // Add an entry to recent activities
    mRecentList.addRecent(tr("Text snippet"), *text, "text", mCurrentTransferBuddy, totalSize);

// Update GUI
// mView->win7()->setProgressState(EcWin7::NoProgress);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    //TODO: check QtQuick 2 alert
    QApplication::alert(mView, 5000);
#endif
    emit receiveCompleted();
}

void GuiBehind::showTextSnippet(QString text, QString sender)
{
    setTextSnippet(text);
    setTextSnippetBuddy(sender);
    setTextSnippetSending(false);
    emit gotoTextSnippet();
}

void GuiBehind::openFile(QString path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void GuiBehind::openDestinationFolder() {
#if defined(Q_OS_ANDROID)
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath()));
    // Get the singleton instance of SharedStorage
    // auto &sharedStorage = android::provider::SharedStorage::instance();

// Define a callback function to handle the result of the document request
// auto callback = [](int /* requestCode */, android::net::Uri uri) {
//     qDebug() << "Document URI:" << uri.toString();
//     // You can now use the URI to access or manipulate the file
// };

// Request to open the document tree (e.g., Downloads directory)
// sharedStorage.openDocumentTree(1 /* requestCode */, callback);

// // Alternatively, you can open a specific document by MIME type
// QStringList mimeTypes = {"text/plain"};
// sharedStorage.openDocument(1 /* requestCode */, callback, mimeTypes);

// You can also create a new document in shared storage
// sharedStorage.createDocument(1 /* requestCode */, callback, "text/plain", "example_file.txt");
#else
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath()));
#endif
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

void GuiBehind::refreshIpList()
{
    mIpAddresses.refreshIpList();
}

void GuiBehind::showSendPage(QString ip)
{
    // Check for a buddy with the provided IP address
    QStandardItem *buddy = mBuddiesList.buddyByIp(ip);
    if (buddy == NULL) return;

    // Update exposed data for the selected user
    mDestBuddy->fillFromItem(buddy);

    // Preventive update of destination buddy
    if (mDestBuddy->ip() == "IP")
        setCurrentTransferBuddy(remoteDestinationAddress());
    else
        setCurrentTransferBuddy(mDestBuddy->username());

    // Preventive update of text send page
    setTextSnippetBuddy(mDestBuddy->username());
    setTextSnippetSending(true);
    setTextSnippet("");

    // Show send UI
    emit gotoSendPage();
}

void GuiBehind::sendDroppedFiles(const QStringList &files)
{
    if (files.isEmpty()) return;

    // Check if there's no selected buddy
    // (but there must be only one buddy in the buddy list)
    if (overlayState() == "")
    {
        if (mBuddiesList.rowCount() != 3) return;
        showSendPage(mBuddiesList.fistBuddyIp());
    }

    QStringList localPaths;

    foreach (const QString &url, files) {
        QUrl fileUrl(url);
        if (fileUrl.isLocalFile()) {
            localPaths.append(fileUrl.toLocalFile());
        }
    }

    // Send files
    QStringList toSend = localPaths;
    startTransfer(toSend);
}

void GuiBehind::sendBuddyDroppedFiles(const QStringList &urls)
{
    if (urls.isEmpty()) return;

    QStringList localPaths;

    foreach (const QString &url, urls) {
        QUrl fileUrl(url);
        if (fileUrl.isLocalFile()) {
            localPaths.append(fileUrl.toLocalFile());
        }
    }

    // qDebug() << localPaths;

    // Send files
    QStringList toSend = localPaths;
    startTransfer(toSend);
}

void GuiBehind::sendSomeFiles(const QStringList &files)
{
    if (files.isEmpty()) return;

    QStringList localPaths;

    foreach (const QString &file, files) {
#if defined(Q_OS_ANDROID)
        // Handle content URI on Android
        QString localFilePath = convertContentUriToFilePath(file);
        if (!localFilePath.isEmpty()) {
            localPaths.append(localFilePath);
        }
#endif
        QUrl fileUrl(file);

        if (fileUrl.isLocalFile()) {
            // If the file is a local file, append it directly
            localPaths.append(fileUrl.toLocalFile());
        }

    }

    if (localPaths.isEmpty()) return;

    // Send files
    startTransfer(localPaths);
}

#if defined(Q_OS_ANDROID)
QString GuiBehind::convertContentUriToFilePath(const QString &uri) {
    // Get the JNI Environment
    QJniEnvironment env;

    // Convert QString to jstring (JNI)
    jstring jUriString = env->NewStringUTF(uri.toUtf8().constData());

    // Find the Android Uri class and the method to parse a string into a Uri
    jclass uriClass = env->FindClass("android/net/Uri");
    jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");

    // Convert the jstring to a Uri object
    jobject jUri = env->CallStaticObjectMethod(uriClass, parseMethod, jUriString);

    // Find the Java FileUtils class
    jclass fileUtilsClass = env->FindClass("idv/coolshou/fileutils/FileUtils");

    // Find the method ID for "getPathFromUri"
    jmethodID getPathFromUriMethod = env->GetStaticMethodID(fileUtilsClass, "getPathFromUri", "(Landroid/content/Context;Landroid/net/Uri;)Ljava/lang/String;");

    // Get the Android context
    jobject context = QNativeInterface::QAndroidApplication::context();

    // Call the Java method with the correct Uri object
    jobject result = env->CallStaticObjectMethod(fileUtilsClass, getPathFromUriMethod, context, jUri);

    // Convert the result (jstring) back to QString
    const char *nativeString = env->GetStringUTFChars(static_cast<jstring>(result), nullptr);
    QString localPath = QString::fromUtf8(nativeString);
    env->ReleaseStringUTFChars(static_cast<jstring>(result), nativeString);

    // Clean up
    env->DeleteLocalRef(jUriString);
    env->DeleteLocalRef(jUri);
    env->DeleteLocalRef(uriClass);
    env->DeleteLocalRef(fileUtilsClass);
    env->DeleteLocalRef(result);

    return localPath;
}
#endif

void GuiBehind::sendAllFiles(const QStringList &files)
{
    if (files.isEmpty()) return;

    QStringList localPaths;

    foreach (const QString &file, files) {
        QUrl fileUrl(file);
        if (fileUrl.isLocalFile()) {
            localPaths.append(fileUrl.toLocalFile());
        }
    }

    // qDebug() << localPaths;

    // Send files
    QStringList toSend = localPaths;
    startTransfer(toSend);
}

void GuiBehind::sendClipboardText()
{
    // Get text to send
    QString text = mClipboard->text();
#ifndef Q_OS_S60
    if (text == "") return;
#else
    if (text == "") {
        setMessagePageTitle(tr("Send"));
        setMessagePageText(tr("No text appears to be in the clipboard right now!"));
        setMessagePageBackState("send");
        emit gotoMessagePage();
        return;
    }
#endif

    // Send text
    startTransfer(text);
}

void GuiBehind::sendText()
{
    // Get text to send
    QString text = textSnippet();
    if (text == "") return;

    // Send text
    startTransfer(text);
}

void GuiBehind::sendScreen()
{
    // Minimize window
    // mView->setWindowState(Qt::WindowMinimized);

    QTimer::singleShot(500, this, SLOT(sendScreenStage2()));
}

void GuiBehind::sendScreenStage2() {

    // Screenshot
    // QPixmap screen = QPixmap::grabWindow(QApplication::desktop()->winId());
    QPixmap screen = QGuiApplication::primaryScreen()->grabWindow(0);

    // Restore window
    // mView->setWindowState(Qt::WindowActive);

    // Salvataggio screenshot in file
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(false);
    tempFile.open();
    mScreenTempPath = tempFile.fileName();
    tempFile.close();
    screen.save(mScreenTempPath, "JPG", 95);

    // Prepare file transfer
    QString ip;
    qint16 port;
    if (!prepareStartTransfer(&ip, &port)) return;

    // Start screen transfer
    mDuktoProtocol.sendScreen(ip, port, mScreenTempPath);
}

void GuiBehind::startTransfer(QStringList files)
{
    // Prepare file transfer
    QString ip;
    qint16 port;
    if (!prepareStartTransfer(&ip, &port)) return;

    // Start files transfer
    mDuktoProtocol.sendFile(ip, port, files);
}

void GuiBehind::startTransfer(QString text)
{
    // Prepare file transfer
    QString ip;
    qint16 port;
    if (!prepareStartTransfer(&ip, &port)) return;

    // Start files transfer
    mDuktoProtocol.sendText(ip, port, text);
}

bool GuiBehind::prepareStartTransfer(QString *ip, qint16 *port)
{
    // Check if it's a remote file transfer
    if (mDestBuddy->ip() == "IP") {

        // Remote transfer
        QString dest = remoteDestinationAddress();

        // Check if port is specified
        if (dest.contains(":")) {

            // Port is specified or destination is malformed...
            static const QRegularExpression rx("^(.*):([0-9]+)$");
            QRegularExpressionMatch match = rx.match(dest);
            if (!match.hasMatch()) {

                // Malformed destination
                setMessagePageTitle(tr("Send"));
                setMessagePageText(tr("Hey, take a look at your destination, it appears to be malformed!"));
                setMessagePageBackState("send");
                emit gotoMessagePage();
                return false;
            }

            // Get IP (or hostname) and port
            *ip = match.captured(1);
            *port = match.captured(2).toInt();
        }
        else {

            // Port not specified, using default
            *ip = dest;
            *port = 0;
        }
        setCurrentTransferBuddy(*ip);
    }
    else {

        // Local transfer
        *ip = mDestBuddy->ip();
        *port = mDestBuddy->port();
        setCurrentTransferBuddy(mDestBuddy->username());
    }

    // Update GUI for file transfer
    setCurrentTransferSending(true);
    setCurrentTransferStats(tr("Connecting..."));
    setCurrentTransferProgress(0);
    // mView->win7()->setProgressState(EcWin7::Normal);
    // mView->win7()->setProgressValue(0, 100);

    emit transferStart();
    return true;
}

void GuiBehind::sendFileComplete(QStringList *files)
{
    // To remove warning
    files = files;

    // Show completed message
    setMessagePageTitle(tr("Send"));
#ifndef Q_OS_S60
    setMessagePageText(tr("Your data has been sent to your buddy!\n\nDo you want to send other files to your buddy? Just drag and drop them here!"));
#else
    setMessagePageText(tr("Your data has been sent to your buddy!"));
#endif
    setMessagePageBackState("send");

    // mView->win7()->setProgressState(EcWin7::NoProgress);

    // Check for temporary file to delete
    if (mScreenTempPath != "") {

        QFile file(mScreenTempPath);
        file.remove();
        mScreenTempPath = "";
    }

    emit gotoMessagePage();
}

void GuiBehind::remoteDestinationAddressHandler()
{
    // Update GUI status
    setCurrentTransferBuddy(remoteDestinationAddress());
    setTextSnippetBuddy(remoteDestinationAddress());
}

// Returns true if the application is ready to accept
// drag and drop for files to send
bool GuiBehind::canAcceptDrop()
{
    // There must be the send page shown and,
    // if it's a remote destination, it must have an IP
    if (overlayState() == "send")
        return !((mDestBuddy->ip() == "IP") && (remoteDestinationAddress() == ""));

    // Or there could be a "send complete" or "send error" message relative to a
    // determinate buddy
    else if ((overlayState() == "message") && (messagePageBackState() == "send"))
        return true;

    // Or there could be just one buddy in the list
    else if (mBuddiesList.rowCount() == 3)
        return true;

    return false;
}

// Handles send error
void GuiBehind::sendFileError(int code)
{
    setMessagePageTitle(tr("Error"));
    setMessagePageText(tr("Sorry, an error has occurred while sending your data...\n\nError code: ") + QString::number(code));
    setMessagePageBackState("send");
    // mView->win7()->setProgressState(EcWin7::Error);

    // Check for temporary file to delete
    if (mScreenTempPath != "") {

        QFile file(mScreenTempPath);
        file.remove();
        mScreenTempPath = "";
    }

    emit gotoMessagePage();
}

// Handles receive error
void GuiBehind::receiveFileCancelled()
{
    setMessagePageTitle(tr("Error"));
    setMessagePageText(tr("An error has occurred during the transfer... The data you received could be incomplete or broken."));
    setMessagePageBackState("");
    // mView->win7()->setProgressState(EcWin7::Error);
    emit gotoMessagePage();
}

// Event handler to catch the "application activate" event
// bool GuiBehind::eventFilter(QObject *obj, QEvent *event)
// {
//     // On application activatio, I send a broadcast hello
//     if (event->type() == QEvent::ApplicationActivate)
//         mDuktoProtocol.sayHello(QHostAddress::Broadcast);

//     return false;
// }

// Changes the current theme color

void GuiBehind::changeThemeColor(QString color)
{
    mTheme.setThemeColor(color);
    mSettings.saveThemeColor(color);
}

QString GuiBehind::getCurrentThemeColor()
{
    return mSettings.themeColor();
}

// Called on application closing event
void GuiBehind::close()
{
    mDuktoProtocol.sayGoodbye();
}

// Reset taskbar progress status
void GuiBehind::resetProgressStatus()
{
    // mView->win7()->setProgressState(EcWin7::NoProgress);
}

// Periodic hello sending
void GuiBehind::periodicHello()
{
    mDuktoProtocol.sayHello(QHostAddress::Broadcast);
}

// Show updates message
void GuiBehind::showUpdatesMessage()
{
    setShowUpdateBanner(true);
}

// Abort current transfer while sending data
void GuiBehind::abortTransfer()
{
    mDuktoProtocol.abortCurrentTransfer();
}

// Protocol confirms that abort has been done
void GuiBehind::sendFileAborted()
{
    resetProgressStatus();
    emit gotoSendPage();
}

// ------------------------------------------------------------
// Property setter and getter

QString GuiBehind::currentTransferBuddy()
{
    return mCurrentTransferBuddy;
}

void GuiBehind::setCurrentTransferBuddy(QString buddy)
{
    if (buddy == mCurrentTransferBuddy) return;
    mCurrentTransferBuddy = buddy;
    emit currentTransferBuddyChanged();
}

int GuiBehind::currentTransferProgress()
{
    return mCurrentTransferProgress;
}

void GuiBehind::setCurrentTransferProgress(int value)
{
    if (value == mCurrentTransferProgress) return;
    mCurrentTransferProgress = value;
    emit currentTransferProgressChanged();
}

QString GuiBehind::currentTransferStats()
{
    return mCurrentTransferStats;
}

void GuiBehind::setCurrentTransferStats(QString stats)
{
    if (stats == mCurrentTransferStats) return;
    mCurrentTransferStats = stats;
    emit currentTransferStatsChanged();
}

QString GuiBehind::textSnippetBuddy()
{
    return mTextSnippetBuddy;
}

void GuiBehind::setTextSnippetBuddy(QString buddy)
{
    if (buddy == mTextSnippetBuddy) return;
    mTextSnippetBuddy = buddy;
    emit textSnippetBuddyChanged();
}

QString GuiBehind::textSnippet()
{
    return mTextSnippet;
}

void GuiBehind::setTextSnippet(QString text)
{
    if (text == mTextSnippet) return;
    mTextSnippet = text;
    emit textSnippetChanged();
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

bool GuiBehind::currentTransferSending()
{
    return mCurrentTransferSending;
}

void GuiBehind::setCurrentTransferSending(bool sending)
{
    if (sending == mCurrentTransferSending) return;
    mCurrentTransferSending = sending;
    emit currentTransferSendingChanged();
}

bool GuiBehind::clipboardTextAvailable()
{
    return mClipboardTextAvailable;
}

QString GuiBehind::remoteDestinationAddress()
{
    return mRemoteDestinationAddress;
}

void GuiBehind::setRemoteDestinationAddress(QString address)
{
    if (address == mRemoteDestinationAddress) return;
    mRemoteDestinationAddress = address;
    emit remoteDestinationAddressChanged();
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

QString GuiBehind::messagePageText()
{
    return mMessagePageText;
}

void GuiBehind::setMessagePageText(QString message)
{
    if (message == mMessagePageText) return;
    mMessagePageText = message;
    emit messagePageTextChanged();
}

QString GuiBehind::messagePageTitle()
{
    return mMessagePageTitle;
}

void GuiBehind::setMessagePageTitle(QString title)
{
    if (title == mMessagePageTitle) return;
    mMessagePageTitle = title;
    emit messagePageTitleChanged();
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

bool GuiBehind::showTermsOnStart()
{
    return mSettings.showTermsOnStart();
}

void GuiBehind::setShowTermsOnStart(bool show)
{
    mSettings.saveShowTermsOnStart(show);
    emit showTermsOnStartChanged();
}

bool GuiBehind::showUpdateBanner()
{
    return mShowUpdateBanner;
}

void GuiBehind::setShowUpdateBanner(bool show)
{
    mShowUpdateBanner = show;
    emit showUpdateBannerChanged();
}

void GuiBehind::setBuddyName(QString name)
{
    qDebug() << "Buddy name is:  " << name;
    mSettings.saveBuddyName(name.replace(' ', ""));
    mDuktoProtocol.updateBuddyName();
    mBuddiesList.updateMeElement();
    emit buddyNameChanged();
}

QString GuiBehind::buddyName()
{
    return mSettings.buddyName();
}

QString GuiBehind::appVersion()
{
    return QGuiApplication::applicationVersion();
}

bool GuiBehind::isTrayIconVisible()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()){
        return trayIcon->isVisible();
    }
    return false;
}

void GuiBehind::setTrayIconVisible(bool bVisible)
{
    if (QSystemTrayIcon::isSystemTrayAvailable()){
        trayIcon->setVisible(bVisible);
    }
}

#if defined(Q_OS_S60)
void GuiBehind::initConnection()
{
    // Connection
    QNetworkConfigurationManager manager;
    const bool canStartIAP = (manager.capabilities() & QNetworkConfigurationManager::CanStartAndStopInterfaces);
    QNetworkConfiguration cfg = manager.defaultConfiguration();
    if (!cfg.isValid() || (!canStartIAP && cfg.state() != QNetworkConfiguration::Active)) return;
    mNetworkSession = new QNetworkSession(cfg, this);
    connect(mNetworkSession, SIGNAL(opened()), this, SLOT(connectOpened()));
    connect(mNetworkSession, SIGNAL(error(QNetworkSession::SessionError)), this, SLOT(connectError(QNetworkSession::SessionError)));
    mNetworkSession->open();
}

void GuiBehind::connectOpened()
{
    mDuktoProtocol.sayHello(QHostAddress::Broadcast);
}

void GuiBehind::connectError(QNetworkSession::SessionError error)
{
    QString msg = tr("Unable to connect to the network (code: ") + QString::number(error) + ").";
    QMessageBox::critical(NULL, tr("Dukto"), msg);
    exit(-1);
}

#endif

void GuiBehind::createActions()
{
    // minimizeAction = new QAction(tr("Mi&nimize"), mView);
    // connect(minimizeAction, SIGNAL(triggered()), mView, SLOT(hide()));

    // maximizeAction = new QAction(tr("Ma&ximize"), mView);
    // connect(maximizeAction, SIGNAL(triggered()), mView, SLOT(showMaximized()));

    // restoreAction = new QAction(tr("&Restore"), mView);
    // connect(restoreAction, SIGNAL(triggered()), mView, SLOT(showNormal()));

    // quitAction = new QAction(tr("&Quit"), mView);
    // connect(quitAction, SIGNAL(triggered()), mView, SLOT(exit()));
}

void GuiBehind::createTrayIcon()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    trayIconMenu = new QMenu(mView);
#else
    trayIconMenu = new QMenu(nullptr);
#endif
    // trayIconMenu->addAction(minimizeAction);
    // //    trayIconMenu->addAction(maximizeAction);
    // trayIconMenu->addAction(restoreAction);
    // trayIconMenu->addSeparator();
    // trayIconMenu->addAction(quitAction);

    // trayIcon = new QSystemTrayIcon(mView);
    // trayIcon->setContextMenu(trayIconMenu);
    // QIcon icon(":/dukto.png");
    // trayIcon->setIcon(icon);
    // //trayIcon->setToolTip("test");
    // connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    //         this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

void GuiBehind::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //single left click
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        // qDebug() << "//TODO:QT5 single left click on systray icon";
        // if (mView->isVisible() || mView->windowState() == Qt::WindowMinimized) {
        //     mView->showNormal(); // Restore the application if it is hidden
        // } else {
        //     mView->hide();
        // }
#else
        if (mView->isHidden() || mView->isMinimized()) {
            mView->showNormal();
        } else {
            mView->hide();
        }
#endif
        break;
    case QSystemTrayIcon::DoubleClick:
        //double click
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}
