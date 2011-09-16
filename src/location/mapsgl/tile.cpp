/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "tile.h"

#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglscenenode.h>

QT_BEGIN_NAMESPACE

Tile::Tile() {}

Tile::Tile(const TileSpec &spec)
        : spec_(spec),
          texture_(0),
          sceneNode_(0),
          bound_(false) {}

bool Tile::operator == (const Tile &rhs) const
{
    return (spec_ == rhs.spec_);
}

void Tile::setTileSpec(const TileSpec &spec)
{
    spec_ = spec;
}

TileSpec Tile::tileSpec() const
{
    return spec_;
}

void Tile::setTexture(QGLTexture2D *texture)
{
    texture_ = texture;
}

QGLTexture2D* Tile::texture() const
{
    return texture_;
}

void Tile::setSceneNode(QGLSceneNode *sceneNode)
{
    sceneNode_ = sceneNode;
}

QGLSceneNode* Tile::sceneNode() const
{
    return sceneNode_;
}

void Tile::bind()
{
    if (bound_)
        return;

    texture_->bind();
//    texture_->clearImage();

    bound_ = true;
}

void Tile::setBound(bool bound)
{
    bound_ = bound;
}

bool Tile::isBound() const
{
    return bound_;
}

QT_END_NAMESPACE