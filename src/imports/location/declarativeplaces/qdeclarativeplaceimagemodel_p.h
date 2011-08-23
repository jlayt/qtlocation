#ifndef QDECLARATIVEPLACEIMAGEMODEL_P_H
#define QDECLARATIVEPLACEIMAGEMODEL_P_H

#include <QtCore/QObject>
#include <QtCore/QAbstractListModel>
#include "qdeclarativeplaceimage_p.h"
#include <qplacemanager.h>

QT_BEGIN_NAMESPACE

class QDeclarativePlace;
class QDeclarativeGeoServiceProvider;
class QGeoServiceProvider;

class QDeclarativePlaceImageModel : public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QDeclarativeParserStatus)

    Q_PROPERTY(QDeclarativePlace *place READ place WRITE setPlace NOTIFY placeChanged)
    Q_PROPERTY(int batchSize READ batchSize WRITE setBatchSize NOTIFY batchSizeChanged)
    Q_PROPERTY(int totalCount READ totalCount NOTIFY totalCountChanged)

public:
    explicit QDeclarativePlaceImageModel(QObject* parent = 0);
    ~QDeclarativePlaceImageModel();

    QDeclarativePlace *place() const;
    void setPlace(QDeclarativePlace *place);

    int batchSize() const;
    void setBatchSize(int batchSize);

    int totalCount() const;

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    enum Roles {
        ImageRole = Qt::UserRole
    };

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void clear();

    void classBegin();
    void componentComplete();

signals:
    void placeChanged();
    void batchSizeChanged();
    void totalCountChanged();

private slots:
    void fetchFinished();

private:
    QDeclarativePlace *m_place;
    int m_batchSize;
    int m_imageCount;
    QMap<int, QDeclarativePlaceImage *> m_images;
    QPlaceContentReply *m_reply;

    bool m_complete;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QDeclarativePlaceImageModel));

#endif