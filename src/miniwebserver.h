#ifndef MINIWEBSERVER_H
#define MINIWEBSERVER_H

#include <QTcpServer>

// FROM: http://doc.qt.nokia.com/solutions/4/qtservice/qtservice-example-server.html

class MiniWebServer : public QTcpServer
{
    Q_OBJECT

public:
    MiniWebServer(int port);

protected:
    virtual void incomingConnection(qintptr handle);

private slots:
    void readClient();
    void discardClient();

private:
    QByteArray mAvatarData;

};

#endif // MINIWEBSERVER_H
