#include "platform.h"

#include <QString>
#include <QHostInfo>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

#if defined(Q_OS_MAC)
#include <QTemporaryFile>
#include <CoreServices/CoreServices.h>
#endif

#if defined(Q_OS_WIN)
#include <windows.h>
#include <lmaccess.h>
#include <QMessageBox>

//typedef struct _USER_INFO_24 {
//  BOOL   usri24_internet_identity;
//  DWORD  usri24_flags;
//  LPWSTR usri24_internet_provider_name;
//  LPWSTR usri24_internet_principal_name;
//  PSID   usri24_user_sid;
//} USER_INFO_24, *PUSER_INFO_24, *LPUSER_INFO_24;


#endif

#if defined(Q_OS_S60)
#define SYMBIAN
#include <QSystemDeviceInfo>
QTM_USE_NAMESPACE
#endif

#if defined(Q_OS_SIMULATOR)
#define SYMBIAN
#endif

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#endif

#include "settings.h"
    QString Platform::username = "";

// Returns the system username
QString Platform::getSystemUsername()
{
    //#if defined(SYMBIAN)
    // Get username from settings
    Settings s;
    qDebug() << "buddyName:" << s.buddyName();
    //return s.buddyName();
    //#else

    // Save in a static variable so that It's always ready
    username = !s.buddyName().isEmpty() ? s.buddyName() : "";
    if (username != "") return username;

#if defined (Q_OS_WIN)
        /*    QString un(getenv("USERNAME"));
    if (un != ""){
        USER_INFO_23 *user_info;
        wchar_t buff[1024];
        int len = un.toWCharArray(buff);
        buff[len] = '\0';
        NET_API_STATUS ret = NetUserGetInfo(NULL, buff, 23, (BYTE**)&user_info);
        if (ret == ERROR_INVALID_LEVEL) {
            QMessageBox::information(NULL, "1", "NOT SUPPORTED", QMessageBox::Ok);
        }
        else if (ret == ERROR_SUCCESS) {
*/            /*QMessageBox::information(NULL, "1", "SUCCESSO", QMessageBox::Ok);
            QString dato = QString::fromWCharArray(user_info->usri24_internet_provider_name);
            QMessageBox::information(NULL, "1", "A" + dato, QMessageBox::Ok);
            dato = QString::fromWCharArray(user_info->usri24_internet_principal_name);
            QMessageBox::information(NULL, "1", "B" + dato, QMessageBox::Ok);*/
        /*            QString dato = QString::fromWCharArray(user_info->usri23_full_name);
            QMessageBox::information(NULL, "1", "A" + dato, QMessageBox::Ok);
            dato = QString::fromWCharArray(user_info->usri23_name);
            QMessageBox::information(NULL, "1", "B" + dato, QMessageBox::Ok);
        }
        else {
            QMessageBox::information(NULL, "1", "ERRORE", QMessageBox::Ok);
        }
*/        /*else {
            QString fullname = QString::fromWCharArray(user_info->usri24_internet_principal_name);
            QMessageBox::information(NULL, "1", fullname, QMessageBox::Ok);
        }*/
        /*fullname = QString::fromWCharArray(user_info->usri23_full_name);
        QMessageBox::information(NULL, "1", fullname, QMessageBox::Ok);*/
//    }

#endif
    /*    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                     << "HOSTNAME.*" << "DOMAINNAME.*";
*/
    // Looking for the username
    QString uname = qgetenv("USERNAME"); //for windows
    if (uname.isEmpty())
        uname = qgetenv("USER");  //for MAC or Linux
    /*    if (uname.isEmpty()) {
        QStringList environment = QProcess::systemEnvironment();
           foreach (QString string, envVariables) {
               int index = environment.indexOf(QRegExp(string));
               if (index != -1) {
                   QStringList stringList = environment.at(index).split('=');
                   if (stringList.size() == 2) {
                       uname = stringList.at(1).toUtf8();
                       break;
                   }
               }
           }
    }
*/

    if (uname.isEmpty())
        uname = "Unknown";

    /* not work on android!!
    QString uname(getenv("USERNAME"));
    if (uname == "") uname = getenv("USER");
    if (uname == "") uname = "Unknown";
    //not work on chinese!!
    uname = uname.left(1).toUpper() + uname.mid(1);
    username = uname;
    */
    return uname;
    //#endif
}

void Platform::updateUsername(const QString &name)
{
    username = name;
    //    qDebug() << "The username changed to:" << username;
}

// Returns the hostname
QString Platform::getHostname()
{
    // Save in a static variable so that It's always ready
    static QString hostname = "";
    if (hostname != "") return hostname;

#if defined(Q_OS_S60)

    QSystemDeviceInfo info;
    hostname = info.model();

#elif defined(Q_OS_ANDROID)
    // Access the Build.MODEL field to get the device model name
    QJniObject model = QJniObject::getStaticObjectField<jstring>("android/os/Build", "MODEL");
    hostname = model.toString();
    qDebug() << "The hostname is" << hostname;
#else

    // Get the hostname
    // (replace ".local" for MacOSX)
    hostname = QHostInfo::localHostName().replace(".local", "");

#endif

    return hostname;
}

// Returns the platform name
QString Platform::getPlatformName()
{
#if defined(Q_OS_WIN)
    return "Windows";
#elif defined(Q_OS_MAC)
    return "Macintosh";
#elif defined(Q_OS_ANDROID)
    return "Android";
#elif defined(Q_OS_UNIX)
    return "Linux";
#elif defined(Q_OS_S60)
    return "Symbian";
#else
    return "Unknown";
#endif
}

// Returns the platform avatar path
QString Platform::getAvatarPath()
{
#if defined(Q_OS_WIN)
    QString username = getSystemUsername().replace("\\", "+");
    QString path = QString(getenv("LOCALAPPDATA")) + "\\Temp\\" + username + ".bmp";
    if (!QFile::exists(path))
        path = getWinTempAvatarPath();
    if (!QFile::exists(path))
        path = QString(getenv("PROGRAMDATA")) + "\\Microsoft\\User Account Pictures\\Guest.bmp";
    if (!QFile::exists(path))
        path = QString(getenv("ALLUSERSPROFILE")) + "\\" + QDir(getenv("APPDATA")).dirName() + "\\Microsoft\\User Account Pictures\\" + username + ".bmp";
    if (!QFile::exists(path))
        path = QString(getenv("ALLUSERSPROFILE")) + "\\" + QDir(getenv("APPDATA")).dirName() + "\\Microsoft\\User Account Pictures\\Guest.bmp";
    return path;
#elif defined(Q_OS_MAC)
    return getMacTempAvatarPath();
#elif defined(Q_OS_UNIX)
    return getLinuxAvatarPath();
#elif defined(Q_OS_S60)
    return "";
#elif defined(Q_OS_ANDROID)
    return getAndroidAvatarPath();
#else
    return "";
#endif
}

// Returns the platform default output path
QString Platform::getDefaultPath()
{
    // Get the default Downloads folder
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

// For android devices, create a Dukto folder inside Downloads folder
#if defined(Q_OS_ANDROID)
    // Return Dowloads as the default path
    return "/sdcard/Download";
#endif

    // On desktop platforms and other platforms, return the Downloads folder as the default path
    return downloadsPath;
}

#if defined(Q_OS_ANDROID)
QString Platform::getAndroidAvatarPath()
{
    QString path="";
    //TODO: how to get android available path
    return path;
}
#endif
#if defined(Q_OS_UNIX)
// Special function for Linux
QString Platform::getLinuxAvatarPath() {
    static QRegularExpression re("^Icon=(.*)$"); // Declare static QRegularExpression object

    QString path;

    // Gnome2 check
    path = QString(getenv("HOME")) + "/.face";
    if (QFile::exists(path)) return path;

    // Gnome3 check
    QFile f("/var/lib/AccountsService/users/" + QString(getenv("USER")));
    if (!f.open(QFile::ReadOnly)) return "";
    QTextStream ts(&f);
    QString line;
    bool found = false;
    while (true) {
        line = ts.readLine();
        if (line.isNull()) break;
        if (line.startsWith("Icon=")) {
            QRegularExpressionMatch match = re.match(line); // Use the static object
            if (match.hasMatch()) {
                QStringList pathlist = match.capturedTexts();
                path = pathlist[1];
                found = true;
                break;
            }
        }
    }
    f.close();
    if (found && QFile::exists(path)) return path;

    // Not found
    return "";
}
#endif

#if defined(Q_OS_MAC)
static QTemporaryFile macAvatar;

// Special function for OSX
QString Platform::getMacTempAvatarPath()
{
    // Get image data from system
    QByteArray qdata;
    CSIdentityQueryRef query = CSIdentityQueryCreateForCurrentUser(kCFAllocatorSystemDefault);
    CFErrorRef error;
    if (CSIdentityQueryExecute(query, kCSIdentityQueryGenerateUpdateEvents, &error)) {
        CFArrayRef foundIds = CSIdentityQueryCopyResults(query);
        if (CFArrayGetCount(foundIds) == 1) {
            CSIdentityRef userId = (CSIdentityRef) CFArrayGetValueAtIndex(foundIds, 0);
            CFDataRef data = CSIdentityGetImageData(userId);
            qDebug() << CFDataGetLength(data);
            qdata.resize(CFDataGetLength(data));
            CFDataGetBytes(data, CFRangeMake(0, CFDataGetLength(data)), (uint8*)qdata.data());
        }
    }
    CFRelease(query);

    // Save it to a temporary file
    macAvatar.open();
    macAvatar.write(qdata);
    macAvatar.close();
    return macAvatar.fileName();
}
#endif

#if defined(Q_OS_WIN)

#include <objbase.h>

#define ARRAYSIZE(a) \
((sizeof(a) / sizeof(*(a))) / \
 static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

    typedef HRESULT (WINAPI*pfnSHGetUserPicturePathEx)(
        LPCWSTR pwszUserOrPicName,
        DWORD sguppFlags,
        LPCWSTR pwszDesiredSrcExt,
        LPWSTR pwszPicPath,
        UINT picPathLen,
        LPWSTR pwszSrcPath,
        UINT srcLen
        );

// Special function for Windows 8
QString Platform::getWinTempAvatarPath()
{
    // Get file path
    CoInitialize(NULL);
    HMODULE hMod = LoadLibrary(L"shell32.dll");
    pfnSHGetUserPicturePathEx picPathFn = (pfnSHGetUserPicturePathEx)GetProcAddress(hMod, (LPCSTR)810);
    WCHAR picPath[500] = {0}, srcPath[500] = {0};
    HRESULT ret = picPathFn(NULL, 0, NULL, picPath, ARRAYSIZE(picPath), srcPath, ARRAYSIZE(srcPath));
    if (ret != S_OK) return "C:\\missing.bmp";
    QString result = QString::fromWCharArray(picPath, -1);
    CoUninitialize();
    return result;
}

#endif
