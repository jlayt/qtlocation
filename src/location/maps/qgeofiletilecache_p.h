/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QGEOFILETILECACHE_P_H
#define QGEOFILETILECACHE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtLocation/qlocationglobal.h>

#include <QObject>
#include <QCache>
#include "qcache3q_p.h"
#include <QSet>
#include <QMutex>
#include <QTimer>

#include "qgeotilespec_p.h"
#include "qgeotiledmappingmanagerengine_p.h"
#include "qabstractgeotilecache_p.h"

#include <QImage>

QT_BEGIN_NAMESPACE

class QGeoMappingManager;

class QGeoTile;
class QGeoCachedTileMemory;
class QGeoFileTileCache;

class QPixmap;
class QThread;

/* This would be internal to qgeofiletilecache.cpp except that the eviction
 * policy can't be defined without it being concrete here */
class QGeoCachedTileDisk
{
public:
    ~QGeoCachedTileDisk();

    QGeoTileSpec spec;
    QString filename;
    QString format;
    QGeoFileTileCache *cache;
};

/* Custom eviction policy for the disk cache, to avoid deleting all the files
 * when the application closes */
class QCache3QTileEvictionPolicy : public QCache3QDefaultEvictionPolicy<QGeoTileSpec,QGeoCachedTileDisk>
{
protected:
    void aboutToBeRemoved(const QGeoTileSpec &key, QSharedPointer<QGeoCachedTileDisk> obj);
    void aboutToBeEvicted(const QGeoTileSpec &key, QSharedPointer<QGeoCachedTileDisk> obj);
};

class Q_LOCATION_EXPORT QGeoFileTileCache : public QAbstractGeoTileCache
{
    Q_OBJECT
public:
    QGeoFileTileCache(const QString &directory = QString(), QObject *parent = 0);
    ~QGeoFileTileCache();

    void setMaxDiskUsage(int diskUsage) Q_DECL_OVERRIDE;
    int maxDiskUsage() const Q_DECL_OVERRIDE;
    int diskUsage() const Q_DECL_OVERRIDE;

    void setMaxMemoryUsage(int memoryUsage) Q_DECL_OVERRIDE;
    int maxMemoryUsage() const Q_DECL_OVERRIDE;
    int memoryUsage() const Q_DECL_OVERRIDE;

    void setMinTextureUsage(int textureUsage) Q_DECL_OVERRIDE;
    void setExtraTextureUsage(int textureUsage) Q_DECL_OVERRIDE;
    int maxTextureUsage() const Q_DECL_OVERRIDE;
    int minTextureUsage() const Q_DECL_OVERRIDE;
    int textureUsage() const Q_DECL_OVERRIDE;

    QSharedPointer<QGeoTileTexture> get(const QGeoTileSpec &spec) Q_DECL_OVERRIDE;

    // can be called without a specific tileCache pointer
    static void evictFromDiskCache(QGeoCachedTileDisk *td);
    static void evictFromMemoryCache(QGeoCachedTileMemory *tm);

    void insert(const QGeoTileSpec &spec,
                const QByteArray &bytes,
                const QString &format,
                QGeoTiledMappingManagerEngine::CacheAreas areas = QGeoTiledMappingManagerEngine::AllCaches) Q_DECL_OVERRIDE;

private:
    void printStats() Q_DECL_OVERRIDE;
    void loadTiles();

    QString directory() const;

    QSharedPointer<QGeoCachedTileDisk> addToDiskCache(const QGeoTileSpec &spec, const QString &filename);
    QSharedPointer<QGeoCachedTileMemory> addToMemoryCache(const QGeoTileSpec &spec, const QByteArray &bytes, const QString &format);
    QSharedPointer<QGeoTileTexture> addToTextureCache(const QGeoTileSpec &spec, const QImage &image);

    static QString tileSpecToFilename(const QGeoTileSpec &spec, const QString &format, const QString &directory);
    static QGeoTileSpec filenameToTileSpec(const QString &filename);

    QCache3Q<QGeoTileSpec, QGeoCachedTileDisk, QCache3QTileEvictionPolicy > diskCache_;
    QCache3Q<QGeoTileSpec, QGeoCachedTileMemory > memoryCache_;
    QCache3Q<QGeoTileSpec, QGeoTileTexture > textureCache_;

    QString directory_;

    int minTextureUsage_;
    int extraTextureUsage_;
};

QT_END_NAMESPACE

#endif // QGEOFILETILECACHE_P_H
