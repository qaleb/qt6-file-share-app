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

    // Load IP list
    QList<QHostAddress> addrs = QNetworkInterface::allAddresses();
    for (int i = 0; i < addrs.length(); i++)
        if ((addrs[i].protocol() == QAbstractSocket::IPv4Protocol)
            && (addrs[i].toString() != "127.0.0.1"))
            addIp(addrs[i].toString());
}
