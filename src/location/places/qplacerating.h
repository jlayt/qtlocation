#ifndef QPLACERATING_H
#define QPLACERATING_H

#include <QSharedDataPointer>
#include "qmobilityglobal.h"

QTM_BEGIN_NAMESPACE

class QPlaceRatingPrivate;

class Q_LOCATION_EXPORT QPlaceRating
{
public:
    QPlaceRating();
    QPlaceRating(const QPlaceRating &other);

    virtual ~QPlaceRating();

    QPlaceRating &operator=(const QPlaceRating &other);

    bool operator==(const QPlaceRating &other) const;
    bool operator!=(const QPlaceRating &other) const {
        return !(other == *this);
    }

    double value() const;
    void setValue(const double &data);
    int count() const;
    void setCount(const int &dcount);

private:
    QSharedDataPointer<QPlaceRatingPrivate> d;
};

QTM_END_NAMESPACE

#endif // QPLACERATING_H