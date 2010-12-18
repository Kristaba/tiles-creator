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

#include "graphicstilescene.h"

GraphicsTileScene::GraphicsTileScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent) {
}


GraphicsTileScene::GraphicsTileScene(QObject *parent) : QGraphicsScene(parent){
}


void GraphicsTileScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        QGraphicsItem *item = itemAt(event->scenePos());
        if (item != 0) {
            GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
            //if (tile != 0) m_oldpos = event->scenePos().toPoint();
            if (tile != 0) m_lastItem = tile;
        }
    }
    QGraphicsScene::mousePressEvent(event);
}


void GraphicsTileScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        QGraphicsItem *item = itemAt(event->scenePos());
        if (item != 0) {
            GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
            if (tile != 0) {
                if (m_lastItem != tile) {
                    QList<QGraphicsItem*> selected = selectedItems();
                    blockSignals(true);
                    for (int i=0; i<selected.size(); i++) selected.at(i)->setSelected(false);
                    blockSignals(false);
                    tile->setSelected(true);
                    //m_oldpos = event->scenePos().toPoint();
                    m_lastItem = tile;
                }
            }
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

/*
void GraphicsTileScene::selectLine(QPoint posA, QPoint posB) {
    int xA = posA.x();
    int yA = posA.y();
    int xB = posB.x();
    int yB = posB.y();


    if ((xA>=0 && xA<width() && yA>=0 && yA<height()) && (xB>=0 && xB<width() && yB>=0 && yB<height())){
        int d, dx, dy, aincr, bincr, xincr, yincr, x, y;

        if (abs(xB - xA) < abs(yB - yA)) {
            // parcours par l'axe vertical

            if (yA > yB) {
              changeInt(xA, xB);
              changeInt(yA, yB);
            }
            xincr = -1;
            if (xB > xA) xincr = 1;
            dy = yB - yA;
            dx = abs(xB - xA);
            d = 2 * dx - dy;
            aincr = 2 * (dx - dy);
            bincr = 2 * dx;
            x = xA;
            y = yA;

            selectOtherItem(QPoint(x, y));

            for (y = yA+1; y <= yB; ++y) {
              if (d >= 0) {
                x += xincr;
                d += aincr;
              } else
                d += bincr;

              selectOtherItem(QPoint(x, y));
            }
        }
        else {
            // parcours par l'axe horizontal

            if (xA > xB) {
              changeInt(xA, xB);
              changeInt(yA, yB);
            }

            yincr = -1;
            if (yB > yA) yincr = 1;
            dx = xB - xA;
            dy = abs(yB - yA);
            d = 2 * dy - dx;
            aincr = 2 * (dy - dx);
            bincr = 2 * dy;
            x = xA;
            y = yA;

            selectOtherItem(QPoint(x, y));

            for (x = xA+1; x <= xB; ++x) {
              if (d >= 0) {
                y += yincr;
                d += aincr;
              } else
                d += bincr;

              selectOtherItem(QPoint(x, y));
            }
        }
    }
}


void GraphicsTileScene::changeInt (int &a, int &b){
    int tmp;
    tmp=a;
    a=b;
    b=tmp;
    return;
}


void GraphicsTileScene::selectOtherItem(QPoint pos) {
    QGraphicsItem *item = itemAt(pos);
    if (item != 0) {
        GraphicsTileItem *tile = qgraphicsitem_cast<GraphicsTileItem*>(item);
        if (tile != 0) {
            if (m_lastItem != tile) {
                item->setSelected(true);
                m_lastItem = tile;
            }
        }
    }
}*/
