#include "ipaddressitemmodel.h"

#include <QHostAddress>
#include <QNetworkInterface>

IpAddressItemModel::IpAddressItemModel() :
    QStandardItemModel(NULL)
{
    QHash<int, QByteArray> roleNames;
    roleNames[Ip] = "ip";
    setItemRoleNames(roleNames);

    refreshIpList();
}

void IpAddressItemModel::addIp(QString ip)
{
    QStandardItem* it = new QStandardItem();
    it->setData(ip, IpAddressItemModel::Ip);
    appendRow(it);
}

void IpAddressItemModel::refreshIpList()
{
    // Clear current IP list
    clear();

    // Get all network interfaces
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // Iterate over each network interface
    for (const QNetworkInterface &interface : interfaces)
    {
        // Skip inactive or loopback interfaces
        if (!(interface.flags() & QNetworkInterface::IsUp) ||
            !(interface.flags() & QNetworkInterface::IsRunning) ||
            (interface.flags() & QNetworkInterface::IsLoopBack))
        {
            continue;
        }

        // Get the list of IP addresses for the interface
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries)
        {
            QHostAddress ip = entry.ip();
            // Add only IPv4 addresses that are not localhost
            if (ip.protocol() == QAbstractSocket::IPv4Protocol &&
                ip.toString() != "127.0.0.1")
            {
                addIp(ip.toString());
            }
        }
    }
}

