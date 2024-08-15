#ifndef RECENTLISTITEMMODEL_H
#define RECENTLISTITEMMODEL_H

#include <QStandardItemModel>

class RecentListItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit RecentListItemModel();
    void addRecent(QString name, QString value, QString type, QString sender, qint64 size);

    enum RecentRoles {
        Name = Qt::UserRole + 1,
        Value,
        Type,
        TypeIcon,
        DateTime,
        Sender,
        Size
    };

signals:

public slots:

};

#endif // RECENTLISTITEMMODEL_H
