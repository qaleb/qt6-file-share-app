#ifndef UPDATESCHECKER_H
#define UPDATESCHECKER_H

#include <QThread>

class QNetworkAccessManager;
class QNetworkReply;

class UpdatesChecker : public QThread
{
    Q_OBJECT
public:
    explicit UpdatesChecker();
    virtual ~UpdatesChecker();
    void run();

signals:
    void updatesAvailable();

public slots:
    void updatedDataReady(QNetworkReply *reply);

private:
    QNetworkAccessManager *mNetworkAccessManager;
};

#endif // UPDATESCHECKER_H
