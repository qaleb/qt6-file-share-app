#include "miniwebserver.h"

#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QRegularExpression>

#include "platform.h"

MiniWebServer::MiniWebServer(int port)
{
    // Load and convert avatar image
    QString path = Platform::getAvatarPath();
    if (path != "")
    {
        QImage img(path);
        QImage scaled = img.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QBuffer tmp(&mAvatarData);
        tmp.open(QIODevice::WriteOnly);
        scaled.save(&tmp, "PNG");

        // Start server
        listen(QHostAddress::Any, port);
    }
}

void MiniWebServer::incomingConnection(int handle)
{
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(handle);
}

void MiniWebServer::readClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegularExpression("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET") {

            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 OK\r\n"
                  "Content-Type: image/png\r\n"
                  "Content-Length: " << mAvatarData.size() << "\r\n"
                                        "\r\n";
            os.flush();

            QDataStream ds(socket);
            ds.writeRawData(mAvatarData.data(), mAvatarData.size());

            socket->close();
            if (socket->state() == QTcpSocket::UnconnectedState) delete socket;
        }
    }
}

void MiniWebServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
