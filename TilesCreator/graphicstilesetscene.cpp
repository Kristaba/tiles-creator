/*  Copyright 2009, 2010 Léo Grange

This file is part of Tiles Creator.

Tiles Creator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Tiles Creator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tiles Creator.  If not, see <http://www.gnu.org/licenses/>. */

#include "graphicstilesetscene.h"

GraphicsTilesetScene::GraphicsTilesetScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent)
{
    toDo();
}


GraphicsTilesetScene::GraphicsTilesetScene(QObject *parent) : QGraphicsScene(parent){
    toDo();
}


void GraphicsTilesetScene::toDo() {
    m_oldTile = 0;
    m_allowedWidget = 0;
    m_dragTileSource = 0;

    m_actionRemove = new QAction(QIcon(":/remove"), tr("Suprimmer"), 0);
    m_actionSetBlock = new QAction(QIcon(":/quit"), tr("Déclarer bloquant"), 0);
    m_actionUnsetBlock = new QAction(tr("Déclarer passant"), 0);
}



void GraphicsTilesetScene::setProjectIndex(const QPersistentModelIndex &index) {
    m_projectIndex = index;
}


QPersistentModelIndex GraphicsTilesetScene::projectIndex(){
    return m_projectIndex;
}


void GraphicsTilesetScene::setAllowedDragWidget(QWidget *w){
    m_allowedWidget = w;
}


QWidget *GraphicsTilesetScene::allowedDragWidget(){
    return m_allowedWidget;
}


void GraphicsTilesetScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem *item = itemAt(event->scenePos());
        if (item != 0) {
            GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
            if (tile != 0) {
                m_oldTile = tile;
                m_oldPos = event->screenPos();
            }
        }
    }
    else if (event->button() == Qt::RightButton) {
    }
    QGraphicsScene::mousePressEvent(event);
}



void GraphicsTilesetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (m_oldTile != 0 && views().size() > 0) {
        QPoint tmp = m_oldPos - event->screenPos();
        if (tmp.manhattanLength() > QApplication::startDragDistance()) {
            QDrag *drag = new QDrag(views().at(0));
            QMimeData *mimeData = new QMimeData();

            /*QImage tileImage(m_oldTile->pixmap().toImage());

            QByteArray data, metadata;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << tileImage;
            mimeData->setData("application/x-tile", data);
            QDataStream metadataStream(&metadata, QIODevice::WriteOnly);
            metadataStream << m_oldTile->tileId() << m_projectIndex.row();
            mimeData->setData("application/x-metatile", metadata);
            QByteArray output;
            QBuffer outputBuffer(&output);
            outputBuffer.open(QIODevice::WriteOnly);
            tileImage.save(&outputBuffer, "PNG");
            mimeData->setData("image/png", output);
            drag->setPixmap(QPixmap::fromImage(tileImage));*/

            QImage tileImage(m_oldTile->pixmap().toImage());

            QByteArray data;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << m_projectIndex.row() << tileImage << m_oldTile->tileId() <<  m_oldTile->blocking();
            mimeData->setData("application/x-tileset-item", data);
            drag->setPixmap(QPixmap::fromImage(tileImage));


            drag->setMimeData(mimeData);
            m_dragTileSource = m_oldTile;
            Qt::DropAction action = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);

            if (action == Qt::MoveAction) {
                m_oldTile->clearTile();
                emit tileModified(m_oldTile);
            }
            m_dragTileSource = 0;
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}



void GraphicsTilesetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (event->button() == Qt::RightButton) {
        QGraphicsItem *item = itemAt(event->scenePos());
        if (item != 0) {
            GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
            if (tile != 0)
                contextMenu(event->screenPos(), tile);
        }
    }
    else if (m_oldTile != 0) {
        if ((m_oldPos - event->screenPos()).manhattanLength() < 2) {
            emit tileSelectorRequested(m_oldTile, event->screenPos());
        }
        m_oldTile = 0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}



void GraphicsTilesetScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
    event->accept();
}



void GraphicsTilesetScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event){
    QGraphicsItem *item = itemAt(event->scenePos());
    if (item != 0) {
        GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
        if ( tile != 0 && views().size() != 0 && ((event->source() == views().at(0))
        || (event->source() == m_allowedWidget))) {
            if (event->mimeData()->hasFormat("application/x-metatile")
            && event->mimeData()->hasFormat("application/x-tile")) {
                QByteArray data(event->mimeData()->data("application/x-tile"));
                QDataStream stream(&data, QIODevice::ReadOnly);
                QImage tmpImage;
                stream >> tmpImage;
                if (tmpImage.size() == tile->pixmap().size()) {
                    QByteArray metadata(event->mimeData()->data("application/x-metatile"));
                    QDataStream metastream(&metadata, QIODevice::ReadOnly);
                    int id, projectRow;
                    metastream >> id >> projectRow;
                    if (projectRow == m_projectIndex.row()) {
                        /*if (event->source() == views().at(0)) event->setDropAction(Qt::MoveAction);
                    else event->setDropAction(event->proposedAction());*/
                        event->acceptProposedAction();
                    }
                    else event->ignore();
                }
                else event->ignore();
            }
            else if (event->mimeData()->hasFormat("application/x-tileset-item")) {
                QByteArray data(event->mimeData()->data("application/x-tileset-item"));
                QDataStream stream(&data, QIODevice::ReadOnly);
                QImage tmpImage;
                int projectRow;
                stream >> projectRow >> tmpImage;
                if (tmpImage.size() == tile->pixmap().size() && projectRow == m_projectIndex.row()) {
                    event->accept();
                }
                else event->ignore();
            }
            else event->ignore();
        }
        else event->ignore();
    }
    else event->ignore();
}



void GraphicsTilesetScene::dropEvent(QGraphicsSceneDragDropEvent *event){
    QGraphicsItem *item = itemAt(event->scenePos());
    if (item != 0) {
        GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
        if (tile != 0) {
            QPixmap swapPixmap;
            int swapId = 0;
            bool swapBlocking = false;
            if (m_dragTileSource != 0) {
                swapPixmap = tile->pixmap();
                swapId = tile->tileId();
                swapBlocking = tile->blocking();
            }
            if (event->mimeData()->hasFormat("application/x-tileset-item")) {
                QByteArray data(event->mimeData()->data("application/x-tileset-item"));
                QDataStream stream(&data, QIODevice::ReadOnly);
                QImage tmpImage;
                int projectRow, id;
                bool isBlocking;
                stream >> projectRow >> tmpImage >> id >> isBlocking;
                tile->setPixmap(QPixmap::fromImage(tmpImage));
                tile->setTileId(id);
                tile->setBlocking(isBlocking);
            }
            else {
                QImage tmpImage;
                QByteArray data(event->mimeData()->data("application/x-tile"));
                QDataStream stream(&data, QIODevice::ReadOnly);
                stream >> tmpImage;
                tile->setPixmap(QPixmap::fromImage(tmpImage));
                QByteArray metadata(event->mimeData()->data("application/x-metatile"));
                QDataStream metastream(&metadata, QIODevice::ReadOnly);
                int id;
                metastream >> id;
                tile->setTileId(id);
                tile->setBlocking(false);
            }
            emit tileModified(tile);
            event->acceptProposedAction();
            if (m_dragTileSource != 0) {
                m_dragTileSource->setPixmap(swapPixmap);
                m_dragTileSource->setTileId(swapId);
                m_dragTileSource->setBlocking(swapBlocking);
                emit tileModified(m_dragTileSource);
            }
        }
    }
}


void GraphicsTilesetScene::contextMenu(QPoint pos, GraphicsTileItem *tile) {
    tile->setSelected(true);
    QMenu menu;
    menu.addAction(m_actionRemove);
    if (tile->blocking()) menu.addAction(m_actionUnsetBlock);
    else menu.addAction(m_actionSetBlock);

    QAction *ret = menu.exec(pos);
    if (ret == m_actionRemove) {
        tile->clearTile();
        emit tileModified(tile);
    }
    else if (ret == m_actionSetBlock) {
        tile->setBlocking(true);
        update(tile->pos().x(), tile->pos().y(), tile->boundingRect().width(), tile->boundingRect().height());
    }
    else if (ret == m_actionUnsetBlock) {
        tile->setBlocking(false);
        update(tile->pos().x(), tile->pos().y(), tile->boundingRect().width(), tile->boundingRect().height());
    }
}
