#include "updateschecker.h"

#include "platform.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSysInfo>
#include <QLocale>
#include <QDebug>

UpdatesChecker::UpdatesChecker() :
    mNetworkAccessManager(NULL)
{
}

UpdatesChecker::~UpdatesChecker()
{
    if (mNetworkAccessManager) delete mNetworkAccessManager;
}

void UpdatesChecker::run()
{
    // Get platform data
    QString osver = "";
    QString os = Platform::getPlatformName().toLower();
#if defined(Q_OS_WIN)
    osver = QString::number(QSysInfo::WindowsVersion);
#elif defined(Q_OS_MAC)
    osver = QString::number(QSysInfo::MacintoshVersion);
#elif defined(Q_OS_S60)
    osver = QString::number(QSysInfo::symbianVersion());
#endif
    QString ver = "6.0";
    QString locale = QLocale::system().name();

    // Send check request
    QNetworkRequest request(QUrl("http://www.msec.it/dukto/r5check.php?ver=" + ver + "&locale=" + locale + "&os=" + os + "&osver=" + osver));
    mNetworkAccessManager = new QNetworkAccessManager();
    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updatedDataReady(QNetworkReply*)));
    mNetworkAccessManager->get(request);

    exec();
}

// Read updates results
void UpdatesChecker::updatedDataReady(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) return;
    if (QString(reply->readAll()) == "") return;
    emit updatesAvailable();
}
