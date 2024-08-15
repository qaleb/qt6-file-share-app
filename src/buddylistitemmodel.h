#ifndef BUDDYLISTITEMMODEL_H
#define BUDDYLISTITEMMODEL_H

#include <QStandardItemModel>

class Peer;
class QUrl;

class BuddyListItemModel : public QStandardItemModel
{
public:
    BuddyListItemModel();
    void addMeElement();
    void addIpElement();
    void addBuddy(QString ip, qint16 port, QString username, QString system, QString platform, QUrl avatarPath);
    void addBuddy(Peer& peer);
    void removeBuddy(QString ip);
    void showSingleBack(int idx);
    void updateMeElement();
    QString buddyNameByIp(QString ip);
    QStandardItem* buddyByIp(QString ip);
    QString fistBuddyIp();

    enum BuddyRoles {
        Ip = Qt::UserRole + 1,
        Port,
        Username,
        System,
        Platform,
        GenericAvatar,
        Avatar,
        OsLogo,
        ShowBack
    };

private:
    QHash<QString, QStandardItem*> mItemsMap;
    QStandardItem* mMeItem;
};

#endif // BUDDYLISTITEMMODEL_H
