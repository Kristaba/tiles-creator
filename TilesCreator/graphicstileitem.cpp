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

#include "graphicstileitem.h"

GraphicsTileItem::GraphicsTileItem(int tileWidth, int tileHeight, int x, int y) : QGraphicsPixmapItem()
{
    m_blocking = false;
    m_showPolicy = false;
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    m_tileId = -1;
    setFlags(ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);
    QImage tmp(m_tileWidth, m_tileHeight, QImage::Format_RGB32);
    tmp.fill(0xFF6DACF8);
    setPixmap(QPixmap::fromImage(tmp));
    setData(XIdRole, x);
    setData(YIdRole, y);
}


void GraphicsTileItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QList<QGraphicsItem*> enabled = scene()->selectedItems();
    //if (!enabled.isEmpty()) enabled.at(0)->setZValue(1);
    if (isSelected())
        scene()->clearSelection();
    else {
        scene()->clearSelection();
        setSelected(true);
        //setZValue(10);
    }
    event->accept();
}


void GraphicsTileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    event->ignore();
}


void GraphicsTileItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QMessageBox::information(scene()->views().at(0), "Tile de map", "Le tile selectionne est indexed "
        + QString::number(m_tileId) + "\nCoordonnes : (" + QString::number(data(XIdRole).toInt())
        + " ; " + QString::number(data(YIdRole).toInt()) + ")");
}



void GraphicsTileItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    //QGraphicsPixmapItem::paint(painter, option, widget);
    QGraphicsPixmapItem tmp(this->pixmap());
    tmp.setSelected(false);
    tmp.paint(painter, option, widget);
    if (isSelected()) {
        //painter->setPen(QPen(QBrush(QColor(0xFF003399)), 1, Qt::DotLine));
        //painter->drawRect(QRectF(1, 0.5, m_tileWidth-1.5, m_tileHeight-1.5));
        painter->fillRect(tmp.boundingRect(), QBrush(QColor(40, 40, 40, 80)));
    }
    if (m_showPolicy && m_blocking) {
        painter->fillRect(tmp.boundingRect(), QBrush(QColor(0xFF, 40, 40, 0xA0), Qt::Dense4Pattern));
    }
}


void GraphicsTileItem::setTileId (int id) {
    m_tileId = id;
}


int GraphicsTileItem::tileId(){
    return m_tileId;
}


void GraphicsTileItem::clearTile() {
    QImage voidImage(m_tileWidth, m_tileHeight, QImage::Format_RGB32);
    voidImage.fill(0xFF6DACF8);
    setPixmap(QPixmap::fromImage(voidImage));
    setTileId(-1);
}


void GraphicsTileItem::setBlocking(bool b) {
    m_blocking = b;
}

bool GraphicsTileItem::blocking() {
return m_blocking;
}

void GraphicsTileItem::setShowPolicy(bool p) {
    m_showPolicy = p;
}

bool GraphicsTileItem::showPolicy() {
    return m_showPolicy;
}
