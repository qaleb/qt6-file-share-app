#ifndef IPADDRESSITEMMODEL_H
#define IPADDRESSITEMMODEL_H

#include <QStandardItemModel>

class IpAddressItemModel : public QStandardItemModel
{
public:
    IpAddressItemModel();
    void refreshIpList();

    enum IpRoles {
        Ip = Qt::UserRole + 1
    };

private:
    void addIp(QString ip);
};

#endif // IPADDRESSITEMMODEL_H
