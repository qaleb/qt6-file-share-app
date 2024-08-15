#include "buddylistitemmodel.h"

#include <QUrl>
#include <QRegExp>

#include "platform.h"
#include "peer.h"

BuddyListItemModel::BuddyListItemModel() :
    QStandardItemModel(NULL)
{
    QHash<int, QByteArray> roleNames;
    roleNames[Ip] = "ip";
    roleNames[Port] = "port";
    roleNames[Username] = "username";
    roleNames[System] = "system";
    roleNames[Platform] = "platform";
    roleNames[GenericAvatar] = "generic";
    roleNames[Avatar] = "avatar";
    roleNames[OsLogo] = "oslogo";
    roleNames[ShowBack] = "showback";
    setItemRoleNames(roleNames);
}

void BuddyListItemModel::addMeElement()
{
    addBuddy("",
             0,
             Platform::getSystemUsername() + " (You)",
             Platform::getHostname(),
             Platform::getPlatformName(),
             QUrl::fromLocalFile(Platform::getAvatarPath()));
}

void BuddyListItemModel::addIpElement()
{
    addBuddy("IP",
             0,
             "IP connection",
             "Send data to a remote device",
             "IP",
             QUrl(""));
}

void BuddyListItemModel::addBuddy(QString ip, qint16 port, QString username, QString system, QString platform, QUrl avatarPath)
{
    QStandardItem* it = NULL;
    bool add = true;
    // qDebug() << "Adding buddy with IP:" << ip.mid(7);
    // Check if the same IP is alreay in the buddy list
    if (mItemsMap.contains(ip)) {
        it = mItemsMap[ip];
        add = false;
    }
    else
        it = new QStandardItem();
    it->setData(ip, BuddyListItemModel::Ip);
    it->setData(port, BuddyListItemModel::Port);
    it->setData(false, BuddyListItemModel::ShowBack);

    // Set (or update) data
    it->setData(username, BuddyListItemModel::Username);
    if (ip != "IP")
        it->setData("at " + system, BuddyListItemModel::System);
    else
        it->setData(system, BuddyListItemModel::System);
    it->setData(platform, BuddyListItemModel::Platform);
    it->setData(avatarPath, BuddyListItemModel::Avatar);

    // Update generic avatar
    if ((platform.toLower() == "symbian") || (platform.toLower() == "android") || (platform.toLower() == "ios") || (platform.toLower() == "blackberry") || (platform.toLower() == "windowsphone"))
        it->setData("qrc:/assets/icons/SmartphoneLogo.png", BuddyListItemModel::GenericAvatar);
    else if (platform.toLower() == "ip")
        it->setData("qrc:/assets/icons/IpLogo.png", BuddyListItemModel::GenericAvatar);
    else
        it->setData("qrc:/assets/icons/PcLogo.png", BuddyListItemModel::GenericAvatar);

    // Update logo
    if (platform.toLower() == "windows")
        it->setData("qrc:/assets/icons/WindowsLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "macintosh")
        it->setData("qrc:/assets/icons/AppleLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "linux")
        it->setData("qrc:/assets/icons/LinuxLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "symbian")
        it->setData("qrc:/assets/icons/SymbianLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "ios")
        it->setData("qrc:/assets/icons/IosLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "windowsphone")
        it->setData("qrc:/assets/icons/WindowsPhoneLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "blackberry")
        it->setData("qrc:/assets/icons/BlackberryLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "android")
        it->setData("qrc:/assets/icons/AndroidLogo.png", BuddyListItemModel::OsLogo);
    else
        it->setData("qrc:/assets/icons/UnknownLogo.png", BuddyListItemModel::OsLogo);

    // Add elemento to the list
    if (add) {
        appendRow(it);
        if (ip != "")
            mItemsMap.insert(ip, it);
        else
            mMeItem = it;
    }
}

void BuddyListItemModel::addBuddy(Peer &peer)
{
    QRegExp rx("^(.*)\\sat\\s(.*)\\s\\((.*)\\)$");
    rx.indexIn(peer.name);
    QStringList data = rx.capturedTexts();

    QString username = data[1];
    QString system = data[2];
    QString platform = data[3];
    QUrl avatarPath = QUrl("http://" + peer.address.toString() + ":" + QString::number(peer.port + 1) + "/dukto/avatar");

    addBuddy(peer.address.toString(),
             peer.port,
             username,
             system,
             platform,
             avatarPath);
}

void BuddyListItemModel::removeBuddy(QString ip)
{
    // Check for element
    if (!mItemsMap.contains(ip)) return;

    // Get element
    QStandardItem* it = mItemsMap[ip];

    // Remove element
    mItemsMap.remove(ip);
    this->removeRow(this->indexFromItem(it).row());
}

void BuddyListItemModel::showSingleBack(int idx)
{
    for (int i = 0; i < rowCount(); i++)
        itemFromIndex(index(i, 0))->setData(false, BuddyListItemModel::ShowBack);
    itemFromIndex(index(idx, 0))->setData(true, BuddyListItemModel::ShowBack);
}

QString BuddyListItemModel::buddyNameByIp(QString ip)
{
    if (!mItemsMap.contains(ip)) return "";
    return mItemsMap.value(ip)->data(BuddyListItemModel::Username).toString();
}

QStandardItem* BuddyListItemModel::buddyByIp(QString ip)
{
    if (!mItemsMap.contains(ip)) return NULL;
    return mItemsMap.value(ip);
}

QString BuddyListItemModel::fistBuddyIp()
{
    if (this->rowCount() < 3) return "";
    return this->index(2, 0).data(BuddyListItemModel::Ip).toString();
}

void BuddyListItemModel::updateMeElement()
{
    mMeItem->setData(Platform::getSystemUsername(), BuddyListItemModel::Username);
}
